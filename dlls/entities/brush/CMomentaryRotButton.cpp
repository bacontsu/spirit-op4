/***
*
*    Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*    
*    This product contains software technology licensed from Id 
*    Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*    All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "CMomentaryRotButton.h"
#include "CBaseButton.h"
#include "util/findentity.h"
#include "util/sound.h"

TYPEDESCRIPTION CMomentaryRotButton::m_SaveData[] =
{
    DEFINE_FIELD( CMomentaryRotButton, m_lastUsed, FIELD_INTEGER ),
    DEFINE_FIELD( CMomentaryRotButton, m_direction, FIELD_INTEGER ),
    DEFINE_FIELD( CMomentaryRotButton, m_returnSpeed, FIELD_FLOAT ),
    DEFINE_FIELD( CMomentaryRotButton, m_start, FIELD_VECTOR ),
    DEFINE_FIELD( CMomentaryRotButton, m_end, FIELD_VECTOR ),
    DEFINE_FIELD( CMomentaryRotButton, m_sounds, FIELD_INTEGER ),
};

IMPLEMENT_SAVERESTORE( CMomentaryRotButton, CBaseToggle );

LINK_ENTITY_TO_CLASS( momentary_rot_button, CMomentaryRotButton );

CMomentaryRotButton* CMomentaryRotButton::Instance(edict_t* pent)
{
    return (CMomentaryRotButton*) GET_PRIVATE(pent);
};

int CMomentaryRotButton::ObjectCaps()
{
    int flags = CBaseToggle::ObjectCaps() & (~FCAP_ACROSS_TRANSITION);
    if (pev->spawnflags & SF_MOMENTARY_DOOR) return flags;
    return flags | FCAP_CONTINUOUS_USE;
}

void CMomentaryRotButton::Spawn()
{
    CBaseToggle::AxisDir( pev );

    if ( pev->speed == 0 )
        pev->speed = 100;

    if ( m_flMoveDistance < 0 ) 
    {
        m_start = pev->angles + pev->movedir * m_flMoveDistance;
        m_end = pev->angles;
        m_direction = 1;        // This will toggle to -1 on the first use()
        m_flMoveDistance = -m_flMoveDistance;
    }
    else
    {
        m_start = pev->angles;
        m_end = pev->angles + pev->movedir * m_flMoveDistance;
        m_direction = -1;        // This will toggle to +1 on the first use()
    }

    if ( pev->spawnflags & SF_MOMENTARY_DOOR )
        pev->solid        = SOLID_BSP;
    else
        pev->solid        = SOLID_NOT;

    pev->movetype    = MOVETYPE_PUSH;
    UTIL_SetOrigin(this, pev->origin);
    SET_MODEL(ENT(pev), STRING(pev->model) );

    const char *pszSound = ButtonSound( m_sounds );
    PRECACHE_SOUND(pszSound);
    pev->noise = ALLOC_STRING(pszSound);
    m_lastUsed = 0;
}

void CMomentaryRotButton::KeyValue( KeyValueData *pkvd )
{
    if (FStrEq(pkvd->szKeyName, "returnspeed"))
    {
        m_returnSpeed = atof(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "sounds"))
    {
        m_sounds = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "axes"))
    {
        UTIL_StringToVector((float*)(pev->movedir), pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CBaseToggle::KeyValue( pkvd );
}

void CMomentaryRotButton::PlaySound()
{
    EMIT_SOUND(ENT(pev), CHAN_VOICE, (char*)STRING(pev->noise), 1, ATTN_NORM);
}

// BUGBUG: This design causes a latentcy.  When the button is retriggered, the first impulse
// will send the target in the wrong direction because the parameter is calculated based on the
// current, not future position.
void CMomentaryRotButton::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
    if (IsLockedByMaster()) return; //LRC
    // the distance between the current angle and the "base" angle.
    pev->ideal_yaw = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_start ) / m_flMoveDistance;

    UpdateAllButtons( pev->ideal_yaw, 1 );

    // Calculate destination angle and use it to predict value, this prevents sending target in wrong direction on retriggering
    Vector dest = pev->angles + pev->avelocity * (pev->nextthink - pev->ltime);
    float value1 = CBaseToggle::AxisDelta( pev->spawnflags, dest, m_start ) / m_flMoveDistance;
    UpdateTarget( value1 );

}

void CMomentaryRotButton::UpdateAllButtons( float value, int start )
{
    // Update all rot buttons attached to my target
    // (this includes myself)
    CBaseEntity *pTarget = NULL;
    for (;;)
    {
        pTarget = UTIL_FindEntityByTarget(pTarget, STRING(pev->target));
        if (FNullEnt(pTarget))
            break;

        if ( FClassnameIs( pTarget->pev, "momentary_rot_button" ) )
        {
            CMomentaryRotButton *pEntity = (CMomentaryRotButton*)pTarget;
            if ( start )
                pEntity->UpdateSelf( value );
            else
                pEntity->UpdateSelfReturn( value );
        }
    }
}

void CMomentaryRotButton::UpdateSelf( float value )
{
    BOOL fplaysound = FALSE;

    if ( !m_lastUsed )
    {
        fplaysound = TRUE;
        m_direction = -m_direction;
    }
    m_lastUsed = 1;

    SetNextThink( 0.1 );

    //LRC check if we're outside the boundaries
    if ( m_direction > 0 && value >= 1.0 )
    {
        pev->avelocity = g_vecZero;
        pev->angles = m_end;
        return;
    }

    if ( m_direction < 0 && value <= 0 )
    {
        pev->avelocity = g_vecZero;
        pev->angles = m_start;
        return;
    }
    
    if (fplaysound)
        PlaySound();

    // HACKHACK -- If we're going slow, we'll get multiple player packets per frame;
    // bump nexthink on each one to avoid stalling
    //LRC- that is to say: our avelocity will get us to the target point in 0.1 secs.
    // If we're being told to move further than that, wait that much longer.
    if ( m_fNextThink < pev->ltime )
        SetNextThink( 0.1 );
    else
    {
        AbsoluteNextThink( m_fNextThink + 0.1 );
    }
    
    pev->avelocity = (m_direction * pev->speed) * pev->movedir;
    SetThink( &CMomentaryRotButton::Off );
}

void CMomentaryRotButton::UpdateTarget( float value )
{
    if (!FStringNull(pev->target))
    {
        CBaseEntity* pTarget = NULL;
        for (;;)
        {
            pTarget = UTIL_FindEntityByTargetname(pTarget, STRING(pev->target));
            if ( !pTarget )
                break;
            pTarget->Use( this, this, USE_SET, value );
        }
    }
}

void CMomentaryRotButton::Off()
{
    pev->avelocity = g_vecZero;
    m_lastUsed = 0;
    if ( FBitSet( pev->spawnflags, SF_MOMENTARY_ROTBUTTON_AUTO_RETURN) && m_returnSpeed > 0 )
    {
        SetThink( &CMomentaryRotButton::Return );
        SetNextThink( 0.1 );
        m_direction = -1;
    }
    else
        SetThink( NULL );
}

void CMomentaryRotButton::Return()
{
    float value = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_start ) / m_flMoveDistance;
    
    UpdateAllButtons( value, 0 );    // This will end up calling UpdateSelfReturn() n times, but it still works right
    if ( value > 0 )
        UpdateTarget( value );
}


void CMomentaryRotButton::UpdateSelfReturn( float value )
{
    if ( value <= 0 )
    {
        pev->avelocity = g_vecZero;
        pev->angles = m_start;
        DontThink();
        SetThink( NULL );
    }
    else
    {
        pev->avelocity = -m_returnSpeed * pev->movedir;
        SetNextThink( 0.1 );
    }
}


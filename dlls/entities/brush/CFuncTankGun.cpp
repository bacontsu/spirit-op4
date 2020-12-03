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

#include "CFuncTankGun.h"
#include "weapons.h"

LINK_ENTITY_TO_CLASS(func_tank, CFuncTankGun);

void CFuncTankGun::Fire(const Vector& barrelEnd, const Vector& forward, entvars_t* pevAttacker)
{
    //    ALERT(at_console, "FuncTankGun::Fire\n");
    int i;

    if (m_fireLast != 0)
    {
        // FireBullets needs gpGlobals->v_up, etc.
        UTIL_MakeAimVectors(pev->angles);

        int bulletCount = (gpGlobals->time - m_fireLast) * m_fireRate;
        if (bulletCount > 0)
        {
            for (i = 0; i < bulletCount; i++)
            {
                switch (m_bulletType)
                {
                case TANK_BULLET_9MM:
                    FireBullets(1, barrelEnd, forward, gTankSpread[m_spread], 4096, BULLET_MONSTER_9MM, 1, m_iBulletDamage, pevAttacker);
                    break;

                case TANK_BULLET_MP5:
                    FireBullets(1, barrelEnd, forward, gTankSpread[m_spread], 4096, BULLET_MONSTER_MP5, 1, m_iBulletDamage, pevAttacker);
                    break;

                case TANK_BULLET_12MM:
                    FireBullets(1, barrelEnd, forward, gTankSpread[m_spread], 4096, BULLET_MONSTER_12MM, 1, m_iBulletDamage, pevAttacker);
                    break;

                default:
                case TANK_BULLET_NONE:
                    break;
                }
            }
            CFuncTank::Fire(barrelEnd, forward, pevAttacker);
        }
    }
    else
        CFuncTank::Fire(barrelEnd, forward, pevAttacker);
}

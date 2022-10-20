// This code was made for HL:E ~ Bacontsu
// trigger_anchor, made to anchor entities to other entities

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "UserMessages.h"

class CPointMessage : public CBaseEntity
{
public:
	bool Save(CSave& save) override;
	bool Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void Spawn() override;
	void Think() override;
	bool KeyValue(KeyValueData* pkvd) override;

	bool m_bIsDeveloperOnly;
	float m_flMaxDistance;
};


LINK_ENTITY_TO_CLASS(point_message, CPointMessage);
TYPEDESCRIPTION CPointMessage::m_SaveData[] =
{
		DEFINE_FIELD(CPointMessage, m_bIsDeveloperOnly, FIELD_BOOLEAN),
		DEFINE_FIELD(CPointMessage, m_flMaxDistance, FIELD_FLOAT),
};
IMPLEMENT_SAVERESTORE(CPointMessage, CBaseEntity);

bool CPointMessage::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "developeronly"))
	{
		m_bIsDeveloperOnly = (bool)atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "radius"))
	{
		m_flMaxDistance = atof(pkvd->szValue);
		return true;
	}

	return CBaseEntity::KeyValue(pkvd);
}

void CPointMessage::Spawn()
{
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CPointMessage::Think()
{
	// if distance isnt defined, lets just use 512 units as a distance
	if (m_flMaxDistance == 0.0f)
		m_flMaxDistance = 512.0f;
	
	// loop through all players
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBaseEntity* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer && pPlayer->IsNetClient())
		{
			// send this entity to client
			MESSAGE_BEGIN(MSG_ONE, gmsgMessage, pPlayer->pev->origin, pPlayer->pev);
			WRITE_COORD(pev->origin.x);
			WRITE_COORD(pev->origin.y);
			WRITE_COORD(pev->origin.z);
			WRITE_STRING(STRING(pev->message));
			WRITE_BYTE(m_bIsDeveloperOnly);
			WRITE_FLOAT(m_flMaxDistance);
			MESSAGE_END();
		}
	}
}
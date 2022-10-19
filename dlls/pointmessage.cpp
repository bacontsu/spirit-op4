// This code was made for HL:E ~ Bacontsu
// trigger_anchor, made to anchor entities to other entities

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "UserMessages.h"

class CPointMessage : public CBaseEntity
{
public:
	void Spawn() override;
	void Think() override;
};


LINK_ENTITY_TO_CLASS(point_message, CPointMessage);


void CPointMessage::Spawn()
{
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CPointMessage::Think()
{
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
			MESSAGE_END();
		}
	}
}
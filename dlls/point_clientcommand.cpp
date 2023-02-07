#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include <string>

class CPointClientCommand : public CBaseEntity
{
public:

	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pInflictor, USE_TYPE useType, float value) override;
	void Think() override;
};

LINK_ENTITY_TO_CLASS(point_clientcommand, CPointClientCommand);

void CPointClientCommand::Spawn()
{
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->nextthink = gpGlobals->time + 0.005f;
}

void CPointClientCommand::Think()
{
}

void CPointClientCommand::Use(CBaseEntity* pActivator, CBaseEntity* pInflictor, USE_TYPE useType, float value)
{
	std::string command = STRING(pev->message) + (std::string) "\n";
	// loop through all players
	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBaseEntity* pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer && pPlayer->IsNetClient())
		{
			CLIENT_COMMAND(pPlayer->edict(), command.c_str());
		}
	}
}
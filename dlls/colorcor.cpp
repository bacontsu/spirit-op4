// This code was made for HL:E ~ Bacontsu
// trigger_anchor, made to anchor entities to other entities

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "UserMessages.h"

class CColorCorrectionSetting : public CBaseEntity
{
public:
	bool Save(CSave& save) override;
	bool Restore(CRestore& restore) override;
	static TYPEDESCRIPTION m_SaveData[];

	void Spawn() override;
	void Use(CBaseEntity* pActivator, CBaseEntity* pInflictor, USE_TYPE useType, float value) override;
	bool KeyValue(KeyValueData* pkvd) override;


	float r, g, b;
	float alpha;
	bool inverse, bw;
};


LINK_ENTITY_TO_CLASS(trigger_colorcorrection, CColorCorrectionSetting);

TYPEDESCRIPTION CColorCorrectionSetting::m_SaveData[] =
{
		DEFINE_FIELD(CColorCorrectionSetting, inverse, FIELD_BOOLEAN),
		DEFINE_FIELD(CColorCorrectionSetting, bw, FIELD_BOOLEAN),
		DEFINE_FIELD(CColorCorrectionSetting, r, FIELD_FLOAT),
		DEFINE_FIELD(CColorCorrectionSetting, g, FIELD_FLOAT),
		DEFINE_FIELD(CColorCorrectionSetting, b, FIELD_FLOAT),
		DEFINE_FIELD(CColorCorrectionSetting, alpha, FIELD_FLOAT),
};

IMPLEMENT_SAVERESTORE(CColorCorrectionSetting, CBaseEntity);

bool CColorCorrectionSetting::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "inverse"))
	{
		inverse = (bool)atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "bw"))
	{
		bw = (bool)atoi(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "r"))
	{
		r = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "g")) // skin is used for content type
	{
		g = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "b")) // skin is used for content type
	{
		b = atof(pkvd->szValue);
		return true;
	}
	else if (FStrEq(pkvd->szKeyName, "alpha")) // skin is used for content type
	{
		alpha = atof(pkvd->szValue);
		return true;
	}

	return CBaseEntity::KeyValue(pkvd);
}

void CColorCorrectionSetting::Spawn()
{
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
}

void CColorCorrectionSetting::Use(CBaseEntity* pActivator, CBaseEntity* pInflictor, USE_TYPE useType, float value)
{
	// Apply the settings
	CVAR_SET_FLOAT("colorcor_inverse", (int)inverse);
	CVAR_SET_FLOAT("colorcor_blackwhite", (int)bw);
	CVAR_SET_FLOAT("colorcor_r", r);
	CVAR_SET_FLOAT("colorcor_g", g);
	CVAR_SET_FLOAT("colorcor_b", b);
	CVAR_SET_FLOAT("colorcor_alpha", alpha);
}
#pragma once
#include "SDK.h"
#define MakePtr( Type, dwBase, dwOffset ) ((Type*)((DWORD)dwBase + (DWORD)dwOffset))

class COffsets
{
private:
	RecvTable* pFindTable(const char* szTableName)
	{
		ClientClass* pClass = gInts.Client->GetAllClasses();
		do
		{
			RecvTable* pRecvTable = pClass->Table;
			if (!pRecvTable)
				continue;
			if (!strcmp(pRecvTable->GetName(), szTableName))
			{
#ifdef DEBUG
				LOGDEBUG("Table '%s' found at: [0x%.8X]", szTableName, (DWORD)pRecvTable);
#endif
				return pRecvTable;
			}
			pClass = pClass->pNextClass;
		} while (pClass);
		LOGDEBUG("Table '%s' NOT found", szTableName);
		return NULL;
	}
	RecvProp* pFindProp(RecvTable* pRecvTable, const char* szPropertyName, int* x)
	{
		if (!pRecvTable) return NULL;
		for (int i = 0; i < pRecvTable->GetNumProps(); i++)
		{
			RecvProp* pRecvProp = pRecvTable->GetProp(i);
			if (!pRecvProp)
				continue;
			if (!strcmp(pRecvProp->GetName(), szPropertyName))
			{
				*x = pRecvProp->GetOffset();
#ifdef DEBUG
				if (*x == 0)
				{
					gBaseAPI.LogToFile("Property '%s' found at: [0x%.8X]", szPropertyName, (DWORD)pRecvProp);
				}
				else
				{
					gBaseAPI.LogToFile("Property '%s' offset: [0x%.2X]", szPropertyName, *x);
				}
#endif
				return pRecvProp;
			}
		}
		LOGDEBUG("Property '%s' NOT found", szPropertyName);
		return NULL;
	}
	int iFindOffset(RecvTable* pRecvTable, const char* szPropertyName)
	{
		int a = NULL;
		pFindProp(pRecvTable, szPropertyName, &a);
		return a;
	}

	int offset_DT_LocalPlayerExclusive__m_iAmmo,
		offset_DT_LocalPlayerExclusive__m_nTickBase,
		offset_DT_LocalPlayerExclusive__m_vecVelocity,
		offset_DT_LocalPlayerExclusive__m_flFriction,
		offset_DT_LocalPlayerExclusive__m_flLaggedMovementValue,

		offset_DT_BaseCombatCharacter__m_hActiveWeapon,
		offset_DT_BaseCombatCharacter__m_hMyWeapons,
		offset_DT_LocalPlayerExclusive__m_vecPunchAngle,
		offset_DT_BaseCombatWeapon__m_hOwner,

		offset_DT_BaseEntity__m_clrRender,
		offset_DT_BaseEntity__m_iTeamNum,
		offset_DT_BaseEntity__m_flSimulationTime,
		offset_DT_BaseEntity__m_hOwnerEntity,

		offset_DT_BasePlayer__m_aimPunchAngle,
		offset_DT_BasePlayer__m_lifeState,
		offset_DT_BasePlayer__m_fFlags,
		offset_DT_BasePlayer__m_iHealth,
		offset_DT_BasePlayer__m_ArmorValue,
		offset_DT_BasePlayer__m_viewPunchAngle,

		offset_DT_BaseCombatWeapon__m_flNextPrimaryAttack;

public:
	void COffsets::DumpAllNetvars()
	{
		ClientClass* pClass = gInts.Client->GetAllClasses();
		do
		{
			RecvTable* pRecvTable = pClass->Table;
			if (!pRecvTable)
				continue;

			LOGDEBUG("Table '%s' found at: [0x%.8X]", pRecvTable->GetName(), (DWORD)pRecvTable);
			for (int i = 0; i < pRecvTable->GetNumProps(); i++)
			{
				RecvProp* pRecvProp = pRecvTable->GetProp(i);
				if (!pRecvProp)
					continue;

				int x = pRecvProp->GetOffset();
				if (x == 0)
				{
					LOGDEBUG("\tProperty '%s' found at: [0x%.8X]", pRecvProp->GetName(), (DWORD)pRecvProp);
					if (strcmp(pRecvProp->GetName(), "baseclass"))
					{
						int tableOffset;
						RecvProp* p = pFindProp(pRecvTable, pRecvProp->GetName(), &tableOffset);
						RecvTable* subTable = p->GetDataTable();
						if (!subTable)
							continue;
						for (int i = 0; i < subTable->GetNumProps(); i++)
						{
							RecvProp* subProp = subTable->GetProp(i);
							if (!subProp)
								continue;
							LOGDEBUG("\t\tProperty '%s' offset: [0x%.2X]", subProp->GetName(), subProp->GetOffset());
						}
					}
				}
				else
				{
					LOGDEBUG("\tProperty '%s' offset: [0x%.2X]", pRecvProp->GetName(), pRecvProp->GetOffset());
				}
			}
			pClass = pClass->pNextClass;
		} while (pClass);
	}
	void COffsets::Initialize()
	{
#ifdef DEBUG
		try
		{
			//gBaseAPI.LogToFile( /*---[BEGIN INIT OFFSETS]---*/XorStr<0xDB,27,0xF4CE7279>("\xF6\xF1\xF0\x85\x9D\xA5\xA6\xAB\xAD\xC4\xAC\xA8\xAE\xBC\xC9\xA5\xAD\xAA\xBE\xAB\xBB\xA3\xAC\xDF\xDE\xD9"+0xF4CE7279).s );
#endif

			RecvTable* tbl = NULL;
			RecvProp* p = NULL;

			tbl = pFindTable("DT_BasePlayer");
			offset_DT_BasePlayer__m_lifeState = iFindOffset(tbl, "m_lifeState");
			offset_DT_BasePlayer__m_fFlags = iFindOffset(tbl, "m_fFlags");
			offset_DT_BasePlayer__m_iHealth = iFindOffset(tbl, "m_iHealth");
			p = pFindProp(tbl, "localdata", &offset_DT_LocalPlayerExclusive__m_nTickBase);
			offset_DT_LocalPlayerExclusive__m_nTickBase = iFindOffset(p->GetDataTable(), "m_nTickBase");
			p = pFindProp(tbl, "localdata", &offset_DT_LocalPlayerExclusive__m_flFriction);
			offset_DT_LocalPlayerExclusive__m_flFriction = iFindOffset(p->GetDataTable(), "m_flFriction");
			p = pFindProp(tbl, "localdata", &offset_DT_LocalPlayerExclusive__m_flLaggedMovementValue);
			offset_DT_LocalPlayerExclusive__m_flLaggedMovementValue = iFindOffset(p->GetDataTable(), "m_flLaggedMovementValue");

			tbl = pFindTable("DT_BaseEntity");
			offset_DT_BaseEntity__m_clrRender = iFindOffset(tbl, "m_clrRender");
			offset_DT_BaseEntity__m_iTeamNum = iFindOffset(tbl, "m_iTeamNum");
			offset_DT_BaseEntity__m_flSimulationTime = iFindOffset(tbl, "m_flSimulationTime");
			offset_DT_BaseEntity__m_hOwnerEntity = iFindOffset(tbl, "m_hOwnerEntity");

			tbl = pFindTable("DT_BaseCombatCharacter");
			offset_DT_BaseCombatCharacter__m_hActiveWeapon = iFindOffset(tbl, "m_hActiveWeapon");
			offset_DT_BaseCombatCharacter__m_hMyWeapons = iFindOffset(tbl, "m_hMyWeapons");

			tbl = pFindTable("DT_BaseCombatWeapon");
			offset_DT_BaseCombatWeapon__m_hOwner = iFindOffset(tbl, "m_hOwner");
			p = pFindProp(tbl, "LocalActiveWeaponData", &offset_DT_BaseCombatWeapon__m_flNextPrimaryAttack);
			offset_DT_BaseCombatWeapon__m_flNextPrimaryAttack = iFindOffset(p->GetDataTable(), "m_flNextPrimaryAttack");

			//m_nSmokeEffectTickBegin
#ifdef DEBUG
//gBaseAPI.LogToFile( /*---[END INIT OFFSETS]---*/XorStr<0x98,25,0x029AAE28>("\xB5\xB4\xB7\xC0\xD9\xD3\xDA\xBF\xE9\xEF\xEB\xF7\x84\xEA\xE0\xE1\xFB\xEC\xFE\xF8\xF1\x80\x83\x82"+0x029AAE28).s );
		}
		catch (...)
		{
			gBaseAPI.LogToFile("Failed NetVars");
		}
#endif
	}


	inline CBaseEntity* COffsets::pGetBaseCombatWeapon(CBaseEntity* pEntity, int index)
	{
		int hMyWeapon = *MakePtr(int, pEntity, offset_DT_BaseCombatCharacter__m_hMyWeapons + index * 4);
		return (CBaseEntity*)(gInts.EntList->GetClientEntityFromHandle(hMyWeapon));
	}
	inline float COffsets::GetNextAttack(CBaseEntity* ent)
	{
		return *MakePtr(float, ent, offset_DT_BaseCombatWeapon__m_flNextPrimaryAttack);
	}
	inline int COffsets::GetTickBase(CBaseEntity* ent)
	{
		return *MakePtr(int, ent, offset_DT_LocalPlayerExclusive__m_nTickBase);
	}

	inline int COffsets::GetHealth(CBaseEntity* pEntity) {
		return *MakePtr(int, pEntity, offset_DT_BasePlayer__m_iHealth);
	}
	inline int COffsets::GetTeamNum(CBaseEntity* pEntity) {
		return *MakePtr(int, pEntity, offset_DT_BaseEntity__m_iTeamNum);
	}
	inline bool COffsets::IsAlive(CBaseEntity* pBaseEntity)
	{
		return LIFE_ALIVE == *MakePtr(BYTE, pBaseEntity, offset_DT_BasePlayer__m_lifeState);
	}
	inline int COffsets::GetFlags(CBaseEntity* pBaseEntity)
	{
		return *MakePtr(int, pBaseEntity, offset_DT_BasePlayer__m_fFlags);
	}
	inline Vector COffsets::m_vecPunchAngle(CBaseEntity* pBaseEntity)
	{
		return *MakePtr(Vector, pBaseEntity, offset_DT_LocalPlayerExclusive__m_vecPunchAngle);
	}
	inline CBaseEntity* COffsets::GetWeaponOwner(CBaseEntity* pWeapon)
	{
		int hOwner = *MakePtr(int, pWeapon, offset_DT_BaseCombatWeapon__m_hOwner);
		return (CBaseEntity*)(gInts.EntList->GetClientEntityFromHandle(hOwner));
	}
	int iCreateMoveOffset = 24, iPaintTraverseOffset = 41, iFrameStageNotifyOffset = 37, iViewSetupOffset = 18, iSceneEndOffset = 9, iFireEventClientSideOffset = 9, iSetViewAnglesOffset = 19, iDoPostScreenEffectsOffset = 44, iLockCursorOffset = 67;
};
extern COffsets gOffsets;
//===================================================================================
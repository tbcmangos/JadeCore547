#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "World.h"

class Boss_Announcer : public PlayerScript
{
	public:
	Boss_Announcer() : PlayerScript ("Boss_Announcer") {}

	void OnCreatureKill(Player* player, Creature* boss) override
	{

		if (boss->isWorldBoss())
		{
			std::string plrname = player->GetName();
			std::string bossname = boss->GetName();	
			char msg[250];
			snprintf(msg, 250, "|CFF7BBEF7[ 击杀公告 ]|r:|cffff0000 %s |r 和他的队友击杀了世界boss |CFF18BE00[%s]|r !!!",plrname.c_str(),bossname.c_str());
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
		}
	}
};

void AddSC_Boss_Announcer()
{
	new Boss_Announcer;
}

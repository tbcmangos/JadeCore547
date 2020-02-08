#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "World.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"


class announce_login : public PlayerScript
{
public:
	announce_login() : PlayerScript("announce_login") { }
 

    std::string announce_login::GetNameLink(Player* player)
    {
        std::string name = player->GetName();
        std::string color;
        switch (player->getClass())
        {
        case CLASS_DEATH_KNIGHT:
            color = "|cffC41F3B";
            break;
        case CLASS_DRUID:
            color = "|cffFF7D0A";
            break;
        case CLASS_HUNTER:
            color = "|cffABD473";
            break;
        case CLASS_MAGE:
            color = "|cff69CCF0";
            break;
        case CLASS_PALADIN:
            color = "|cffF58CBA";
            break;
        case CLASS_PRIEST:
            color = "|cffFFFFFF";
            break;
        case CLASS_ROGUE:
            color = "|cffFFF569";
            break;
        case CLASS_SHAMAN:
            color = "|cff0070DE";
            break;
        case CLASS_WARLOCK:
            color = "|cff9482C9";
            break;
        case CLASS_WARRIOR:
            color = "|cffC79C6E";
            break;
        case CLASS_MONK:
            color = "|cffFF8C09";
            break;
        default:
            color = "|cffFFFFFF";
            break;
        }
        return "|Hplayer:" + name + "|h|cffFFFFFF[" + color + name + "|cffFFFFFF]|h|r";
    }



	void OnLogin(Player* player) override
	{
        std::string msg = "";
        if (player->GetTeam() == ALLIANCE)
        {
            msg += "|cff0000ff [联盟]  ";
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
        }
        else if (player->GetTeam() == PANDAREN_NEUTRAL)
        {
            msg += "|cffABD473 [熊猫]  ";
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
        }
        else
        {
            msg += "|cffff0000 [部落]  ";
            msg += GetNameLink(player);
            msg += " |cffFFD800";
        }
        msg += " 上线    ";

        char buff[2048];
        sprintf(buff, "[上线提示] %s ", msg.c_str());

		sWorld->SendServerMessage(SERVER_MSG_STRING, buff);

	}
};
 
void AddSC_announce_login()
{
	new announce_login();
}

/*
* Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*ScriptData
Name: Custom_Chat
%Complete: 50% ?
ToDO: See if possible to make it so that names are clickable/linkable
and See if possible to make faction specific/only chat so that only ally can see ally and horde can see horde etc
and possibly make it so that you can chat with console too
Comment: Custom Chat,
Made by Phantom123 of AC-Web
With help of:
Andy of AC-Web
Jameyboor of AC-Web
Mindsear of AC-Web
Category:Custom Script
EndScriptData*/


#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "World.h"
#include "WorldSession.h"
#include "Channel.h"
#include "ChannelMgr.h"
#include "Player.h"
#include "Language.h"
#include "Common.h"

#define FACTION_SPECIFIC 0

std::string GetNameLink(Player* player)
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

class Custom_Chat : public CommandScript
{
public:
    Custom_Chat() : CommandScript("Custom_Chat") { }


    ChatCommand* GetCommands() const
    {
        static ChatCommand WorldChatCommandTable[] =
        {
            {"sj",	SEC_PLAYER,		true,		&HandleWorldChatCommand,	"", NULL},
            {NULL,		0,				false,		NULL,						"", NULL}
        };

        return WorldChatCommandTable;
    }

    static bool HandleWorldChatCommand(ChatHandler * handler, const char * args)
    {

        if (!handler->GetSession()->GetPlayer()->CanSpeak())
            return false;
        std::string temp = args;

        if (!args || temp.find_first_not_of(' ') == std::string::npos)
            return false;

        std::string msg = "";
        Player * player = handler->GetSession()->GetPlayer();

        switch (player->GetSession()->GetSecurity())
        {
            // Player
        case SEC_PLAYER:

            if (player->GetTeam() == ALLIANCE)
            {
                msg += "|cff0000ff [ÁªÃË]  ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            else if (player->GetTeam() == PANDAREN_NEUTRAL)
            {
                msg += "|cffABD473 [ÐÜÃ¨]  ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            else
            {
                msg += "|cffff0000 [²¿Âä]  ";
                msg += GetNameLink(player);
                msg += " |cffFFD800";
            }
            break;
            // Moderator/trial 
        case SEC_MODERATOR:
            msg += "|cffff8a00[Mod] ";
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
            break;
            // GM
        case SEC_GAMEMASTER:
            msg += "|cff00ffff[GM] ";
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
            break;
            // Admin
        case SEC_ADMINISTRATOR:
            msg += "|cfffa9900[Admin] ";
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
            break;
        default:
            msg += "|cfffa9900 ";//255 or other?
            msg += GetNameLink(player);
            msg += " |cfffaeb00";
            break;
        }

        msg += args;

        char buff[2048];
        sprintf(buff, "[CHAT] %s ", msg.c_str());

        if (FACTION_SPECIFIC)
        {
            SessionMap sessions = sWorld->GetAllSessions();
            for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
                if (Player* plr = itr->second->GetPlayer())
                    if (plr->GetTeam() == player->GetTeam())
                    {
                        //sWorld->SendGlobalText(ss.str().c_str(), nullptr);
                        sWorld->SendServerMessage(SERVER_MSG_STRING, buff);
                    }
                        
        }
        else
        {
            //sWorld->SendGlobalText(ss.str().c_str(), nullptr);
            sWorld->SendServerMessage(SERVER_MSG_STRING, buff);
        }

        return true;
    }
};

void AddSC_Custom_Chat()
{
    new Custom_Chat();
}
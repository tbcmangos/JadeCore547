#include "ScriptPCH.h"

static const uint32 LOC_OFFSET = 1200;
bool premium = false;
int32 cost = 10000;

struct TeleportInfo
{
    float x, y, z, orientation;
    uint32 mapid;
};

static const TeleportInfo locations[] =
{
    //      x          y         z      o   map
    // Alliance capitals
    {-8802.53f,   636.65f,   94.45f, 3.65f,  0},  // 00 Stormwind
    {10037.42f,   2499.61f, 1318.43f, 3.30f, 1},  // 01 Darnassus
    {-3868.21f, -11640.79f, -137.66f, 2.51f,530}, // 02 Exodar
    {-4823.27f,  -1161.57f,  502.21f,  1.4f, 0},  // 03 Ironforge
    {821.36f,  253.14f,  503.92f,  0.63f,  870},  // 04 Shrine of Seven Stars
    {-3826.78f,  -4519.99f,  10.68f,  0.89f, 1},  // 05 Theramore
    // Horde capitals
    { 1569.97f, -4397.41f,   16.05f,  0.5f,  1},  // 06 Orgrimmar
    {9928.50f,   -7131.41f,  47.94f, 4.0f, 530},  // 07 Undercity
    {1771.33f,   62.51f,   -46.32f,   0.78f, 0},  // 08 Silvermoon City
    {-1277.37f,   124.8f,   131.29f,  5.22f, 1},  // 09 Thunder Bluff
    {1678.44f,   931.67f,  471.43f,  3.28f,870},  // 10 Shrine of Two Moons
    // Neutral cities
    {5813.94f,  448.89f,   658.76f, 1.37f, 571},  // 11 Dalaran
    {-1834.46f,  5304.79f, -12.42f, 5.05f, 530},  // 12 Shattrath City
    // PvP zones
    {-13289.15f,  107.71f,  23.21f,  1.06f,  0},  // 13 Gurubashi Arena
    {5051.58f,  2848.65f,  393.15f, 6.26f, 571},  // 14 Wintergrasp
    {-1989.27f,  6571.08f, 10.67f,  2.32f, 530},  // 15 Nagrand Arena
    {-1062.44f,  985.93f, 123.43f,  3.2f,  732},  // 16 Tol Barad
    // World zones
    {-11847.26f, -3200.39f, -29.1f,  3.28f,  0},  // 17 Dark Portal
    {-7203.69f,  -3760.63f,  9.05f,  2.40f,  1},  // 18 Gadgetzan
    {7802.98f,  -2429.34f,  487.57f, 2.90f,  1},  // 19 Moonglade
    {3036.45f,  3699.34f,  143.81f, 5.54f, 530},  // 20 Area 52
    {5747.02f,  1162.21f,  1.11f,   0.09f, 870},  // 21 Isle of Giants
    {6871.39f, 5407.79f,   47.86f, 5.48f, 1064},  // 22 Isle of Thunder
    {12752.59f, -6927.9f,  12.24f, 2.53f,  530},  // 23 Isle of Quel'Danas
    {-650.54f, -4845.78f,   2.06f, 4.74f,  870},  // 24 Timeless Isle
    {2182.87f, 4818.48f,  181.96f, 2.19f,  870},  // 25 Niuzao Temple
    // Dungeons
    {3526.61f, 5157.02f, -1.48f,   4.80f,  530},  // 26 Gruul's Lair
    {-4882.03f, -4254.14f, 827.77f, 2.04f,   0},  // 27 Bastion of Twilight
    {3665.89f, -1269.14f, 243.52f,   6.2f, 571},  // 28 Naxxramas
    {3099.22f,  1515.26f, 190.3f,  4.64f,  530},  // 29 The Eye
    {-3602.68f, 327.23f,  38.74f,  3.48f,  530},  // 30 Black Temple
    {3781.81f,  6953.65f, 104.82f, 0.5f,   571},  // 31 Nexus
    {-8085.42f, 1526.87f,  2.62f,   3.18f,  1},   // 32 Ahn'Qiraj
    {-8374.64f, -4259.58f, -206.23f, 0.73f, 1},   // 33 Caverns of Time
    {-11125.66f, -2013.55f, 47.22f,  0.62f, 0},   // 34 Karazhan
    {750.83f,   6866.02f, -69.29f, 0.02f, 530},   // 35 Serpentshrine Cavern
    {-4671.09f, 3703.31f,  46.07f, 3.78f,   1},   // 36 Onyxia's Lair
    {8518.28f,  723.12f, 558.25,   1.63f, 571},   // 37 Argent Tournament
    {5831.17f,  2214.25f, 636.04f, 3.18f, 571},   // 38 Icecrown Citadel
    {9345.56f, -1114.88f, 1245.09f, 3.11f, 571},  // 39 Ulduar
    {3448.0f,   261.55f,   -110.16f,  0.1f, 571}, // 40 Obsidian Sanctum
    {-7613.76f, -1224.3f, 232.95f,  3.05f,  0},   // 41 Blackrock Mountain
    {3993.59f,  -2957.46f, 1002.55f, 5.15f,  1},  // 42 Sulfuron Spire
    {1265.05f, -4842.05f, 215.77f, 3.8f,  571},   // 43 Utgarde
    {-3335.9f, 4940.86f, -100.40f, 4.55f, 530},   // 44 Auchindoun
};

class Teleguy : public CreatureScript
{
public:
    Teleguy() : CreatureScript("teleguy") {}

    void TeleportTo(Player *player, Creature *creature, uint32 action)
    {
        uint32 idx = action - LOC_OFFSET;
        player->CLOSE_GOSSIP_MENU();
        player->TeleportTo(locations[idx].mapid, locations[idx].x, locations[idx].y, locations[idx].z, locations[idx].orientation);
    }

    void ShowCapitals(Player* player, Creature *creature)
    {
        player->PlayerTalkClass->ClearMenus();
        if( player->GetTeam() == ALLIANCE )
        {
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_human:25|t 暴风城  "   , GOSSIP_SENDER_MAIN, LOC_OFFSET + 0);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_nightelf:25|t 达纳苏斯  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 1);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_draenei:25|t 埃索达  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 2);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_dwarf:25|t 铁炉堡  "   , GOSSIP_SENDER_MAIN, LOC_OFFSET + 3);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_bannerpvp_02:25|t 七星殿  "        , GOSSIP_SENDER_MAIN, LOC_OFFSET + 4);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_bannerpvp_02:25|t 塞拉摩  "                    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 5);
        }
        else
        {
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_orc:25|t 奥格瑞玛  "            , GOSSIP_SENDER_MAIN, LOC_OFFSET + 6);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_bloodelf:25|t 银月城  "     , GOSSIP_SENDER_MAIN, LOC_OFFSET + 7);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_scourge:25|t 幽暗城  "        , GOSSIP_SENDER_MAIN, LOC_OFFSET + 8);
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_misc_tournaments_banner_tauren.png:25|t 雷霆崖  " , GOSSIP_SENDER_MAIN, LOC_OFFSET + 9);    
            player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Inv_bannerpvp_01:25|t 双月殿  "                 , GOSSIP_SENDER_MAIN, LOC_OFFSET + 10);   
        }
        player->ADD_GOSSIP_ITEM( 7, "[  主菜单  ] ->"                    , GOSSIP_SENDER_MAIN, 5550);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowNeutralCities(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Spell_Arcane_PortalDalaran:25|t 达拉然  "         , GOSSIP_SENDER_MAIN, LOC_OFFSET + 11);
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Spell_Arcane_PortalShattrath:25|t 沙塔斯城  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 12);
        player->ADD_GOSSIP_ITEM( 7, "[  主菜单  ] ->"           , GOSSIP_SENDER_MAIN, 5550);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowPvPZones(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM( 9, "|TInterface\\icons\\Achievement_arena_2v2_2.png:25|t 古拉巴什竞技场  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 13);
        player->ADD_GOSSIP_ITEM( 9, "|TInterface\\icons\\ACHIEVEMENT_WIN_WINTERGRASP:25|t 冬拥湖  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 14);
        player->ADD_GOSSIP_ITEM( 9, "|TInterface\\icons\\Achievement_Zone_Nagrand_01:25|t 纳格兰竞技场 "  , GOSSIP_SENDER_MAIN, LOC_OFFSET + 15);
        player->ADD_GOSSIP_ITEM( 9, "|TInterface\\icons\\Achievement_Zone_TolBarad:25|t 巴拉德  "        , GOSSIP_SENDER_MAIN, LOC_OFFSET + 16);
        player->ADD_GOSSIP_ITEM( 7, "[  主菜单  ] ->"                    , GOSSIP_SENDER_MAIN, 5550);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowWorldZones(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM( 2, "  黑暗之门  "       , GOSSIP_SENDER_MAIN, LOC_OFFSET + 17);
        player->ADD_GOSSIP_ITEM( 2, "  加基森  "         , GOSSIP_SENDER_MAIN, LOC_OFFSET + 18);
        player->ADD_GOSSIP_ITEM( 2, "  月光林地  "         , GOSSIP_SENDER_MAIN, LOC_OFFSET + 19);
        player->ADD_GOSSIP_ITEM( 2, "  52区域  "           , GOSSIP_SENDER_MAIN, LOC_OFFSET + 20);
        player->ADD_GOSSIP_ITEM( 2, "  巨人岛  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 21);
        player->ADD_GOSSIP_ITEM( 2, "  雷神岛  "   , GOSSIP_SENDER_MAIN, LOC_OFFSET + 22);
        player->ADD_GOSSIP_ITEM( 2, "  奎尔达纳斯岛  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 23);
        player->ADD_GOSSIP_ITEM( 2, "  永恒之岛  "     , GOSSIP_SENDER_MAIN, LOC_OFFSET + 24);
        player->ADD_GOSSIP_ITEM( 2, "  砮皂寺  "     , GOSSIP_SENDER_MAIN, LOC_OFFSET + 25);
        player->ADD_GOSSIP_ITEM( 7, "[  主菜单  ] ->"                    , GOSSIP_SENDER_MAIN, 5550);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowDungeons(Player* player, Creature* creature, int page)
    {
        player->PlayerTalkClass->ClearMenus();
        if( page == 1 ) {
            player->ADD_GOSSIP_ITEM( 2, "  格鲁尔之巢穴  "       , GOSSIP_SENDER_MAIN, LOC_OFFSET + 26);
            player->ADD_GOSSIP_ITEM( 2, "  暮光堡垒  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 27);
            player->ADD_GOSSIP_ITEM( 2, "  纳克萨玛斯  "          , GOSSIP_SENDER_MAIN, LOC_OFFSET + 28);
            player->ADD_GOSSIP_ITEM( 2, "  风暴之眼  "            , GOSSIP_SENDER_MAIN, LOC_OFFSET + 29);
            player->ADD_GOSSIP_ITEM( 2, "  黑暗神殿  "       , GOSSIP_SENDER_MAIN, LOC_OFFSET + 30);
            player->ADD_GOSSIP_ITEM( 2, "  魔枢  "              , GOSSIP_SENDER_MAIN, LOC_OFFSET + 31);
            player->ADD_GOSSIP_ITEM( 2, "  安其拉  "          , GOSSIP_SENDER_MAIN, LOC_OFFSET + 32);
            player->ADD_GOSSIP_ITEM( 2, "  时光之穴  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 33);
            player->ADD_GOSSIP_ITEM( 7, "[  更多  ] ->"          , GOSSIP_SENDER_MAIN, 5552);
        } else if( page == 2) {
            player->ADD_GOSSIP_ITEM( 2, "  卡拉赞  "   , GOSSIP_SENDER_MAIN, LOC_OFFSET + 34);
            player->ADD_GOSSIP_ITEM( 2, "  毒蛇神殿  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 35);
            player->ADD_GOSSIP_ITEM( 2, "  奥妮克希亚的巢穴  "       , GOSSIP_SENDER_MAIN, LOC_OFFSET + 36);
            player->ADD_GOSSIP_ITEM( 2, "  银色竞技场  "  , GOSSIP_SENDER_MAIN, LOC_OFFSET + 37);
            player->ADD_GOSSIP_ITEM( 2, "  冰冠堡垒  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 38);
            player->ADD_GOSSIP_ITEM( 2, "  奥杜尔  "              , GOSSIP_SENDER_MAIN, LOC_OFFSET + 39);
            player->ADD_GOSSIP_ITEM( 2, "  黑曜石圣殿  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 40);
            player->ADD_GOSSIP_ITEM( 7, "[  更多  ] ->"           , GOSSIP_SENDER_MAIN, 5553);
            player->ADD_GOSSIP_ITEM( 7, "[  返回  ] ->"           , GOSSIP_SENDER_MAIN, 5551);
        } else if( page == 3) {
            player->ADD_GOSSIP_ITEM( 2, "  黑石山脉  ", GOSSIP_SENDER_MAIN, LOC_OFFSET + 41);
            player->ADD_GOSSIP_ITEM( 2, "  萨弗隆尖塔  "    , GOSSIP_SENDER_MAIN, LOC_OFFSET + 42);
            player->ADD_GOSSIP_ITEM( 2, "  乌特加德  "           , GOSSIP_SENDER_MAIN, LOC_OFFSET + 43);
            player->ADD_GOSSIP_ITEM( 2, "  奥金顿  "        , GOSSIP_SENDER_MAIN, LOC_OFFSET + 44);
            player->ADD_GOSSIP_ITEM( 7, "[  返回  ] ->"         , GOSSIP_SENDER_MAIN, 5552);
        }
        player->ADD_GOSSIP_ITEM( 7, "[  主菜单  ] ->"                     , GOSSIP_SENDER_MAIN, 5550);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }

    void ShowMainMenu(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Achievement_doublejeopardy:25|t [  主要城市  ]  ->"      , GOSSIP_SENDER_MAIN, 5560);
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Achievement_bg_takexflags_ab:25|t [  中立城市  ]  ->", GOSSIP_SENDER_MAIN, 5561);
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Achievement_bg_winsoa:25|t [  世界区域  ]  ->"   , GOSSIP_SENDER_MAIN, 5563);
        player->ADD_GOSSIP_ITEM( 2, "|TInterface\\icons\\Achievement_bg_killxenemies_generalsroom:25|t [  副本  ]  ->"      , GOSSIP_SENDER_MAIN, 5551);
        player->ADD_GOSSIP_ITEM( 9, "|TInterface\\icons\\Achievement_bg_winwsg:25|t [  PVP区域  ]  ->"     , GOSSIP_SENDER_MAIN, 5562);
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    }
    
    bool OnGossipHello(Player* player, Creature* creature)
    {
        ChatHandler handler(player);

        if (player)
            ShowMainMenu(player, creature);

        return true;
    }
    
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (sender != GOSSIP_SENDER_MAIN)
            return false;

        if(!player->getAttackers().empty())
        {
            player->CLOSE_GOSSIP_MENU();
            creature->MonsterSay("你正在战斗中  ", LANG_UNIVERSAL, NULL);
            return true;
        }

        if( action >= LOC_OFFSET && action < LOC_OFFSET + sizeof(locations) )
        {
            TeleportTo(player, creature, action);
            return true;
        }
        
        switch( action ){
            case 5550: ShowMainMenu(player, creature); break;
            case 5551: ShowDungeons(player, creature, 1); break;
            case 5552: ShowDungeons(player, creature, 2); break;
            case 5553: ShowDungeons(player, creature, 3); break;
            case 5560: ShowCapitals(player, creature); break;
            case 5561: ShowNeutralCities(player, creature); break;
            case 5562: ShowPvPZones(player, creature); break;
            case 5563: ShowWorldZones(player, creature); break;
        }

        return true;
    }
};

void AddSC_teleguy()
{
    new Teleguy();
}

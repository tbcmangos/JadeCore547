#include "ScriptPCH.h"
#include <map>

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


//dungeons

//struct TeleInfo
//{
//	std::string text;
//	WorldLocation loc;
//};
//
//std::map<uint32, std::vector<TeleInfo>> teleMenu
//{
//    // Arena:
//    { 6,{ { "White Tiger Temple Arena",{ 870, 3599.708252f, 543.187927f, 615.807190f } },
//        { "Arena in the Twilight Highlands",{ 0, -4208.134766f, -5112.048828f, 25.845881f } },
//        { "Arena in zul'Drak",{ 571, 5793.524902f, -3020.279785f, 286.290100f } },
//        { "Nagrand Arena",{ 530, -723.467346f, 7932.981445f, 58.751961f } },
//        { "Tanaris Arena",{ 1, -7113.511719f, -3793.874756f, 8.410527f } },
//        { "Gurubashi Arena",{ 0, -13226.339844f, 231.952881f, 33.284954f } },
//        { "Arena in the Circle of Blood",{ 530, 2838.452393f, 5927.564941f, 11.070838f } },
//        { "Nagrand Arena",{ 530, -2045.597168f, 6653.827148f, 13.054525f } },
//        { "New Moon Island",{ 974, -3980.063477f, 6231.134277f, 14.134694f } } } },
//    //  Pandaria:
//    { 5,{ { "Siege of Orgrimmar (JO)",{ 870, 1198.294434f, 648.816833f, 347.513977f } },
//        { "Mogu'shan Dungeons (PMS)",{ 870, 3984.708252f, 1107.497192f, 497.145966f } },
//        { "Throne of the Thunder (PG)",{ 1064, 7241.394043f, 5038.210938f, 76.220665f } },
//        { "Heart of Fear (SS)",{ 870, 170.666321f, 4041.114990f, 255.914886f } },
//        { "Terrace of Eternal Spring (PMT)",{ 870, 953.370483f, -58.449921f, 510.107422f } },
//        { "The Gate of the Setting Sun",{ 870, 682.647339f, 2079.921143f, 371.710358f } },
//        { "Mogo'shan Palace",{ 870, 1390.274536f, 439.529663f, 479.034119f } },
//        { "The hop-ins of the Exuberant Porters",{ 870, -691.994080f, 1269.524292f, 136.084671f } },
//        { "Shado Pan Monastery",{ 870, 3627.857422f, 2536.668213f, 769.950073f } },
//        { "Siege of the Temple of Nyuzao",{ 870, 1419.895508f, 5082.514160f, 126.282410f } },
//        { "Jade Snake Temple",{ 870, 961.467407f, -2463.143066f, 180.581985f } },
//        { "Tavern \"In the Fog\"",{ 870, 812.400696f, -178.757324f, 415.131195f } },
//        { "Anglerov Village",{ 870, -1479.061157f, -231.548401f, 5.112297f } },
//        { "Timeless island",{ 870, -650.147034f, -4899.938477f, 2.056325f } },
//        { "Giant Island",{ 870, 5751.615723f, 1162.714722f, 1.559572f } },
//        { "Thunder Island",{ 1064, 6800.775879f, 5478.339844f, 29.928785f } }} },
//
//    // Cataclysm:
//    { 4,{ { "Dragon Soul (DD)",{ 1, -8273.000977f, -4515.538574f, -219.056763f } },
//        { "Baradin Fortress (KB)",{ 732, -1204.470093f, 1081.330078f, 120.354973f } },
//        { "Fiery Spaces (OP)",{ 1, 3990.985352f, -2951.045654f, 1002.547424f } },
//        { "Twilight Bastion (SB)",{ 0, -4888.555176f, -4242.727539f, 827.762878f } },
//        { "The Hard Wing of Darkness (TKT)",{ 0, -7538.248047f, -1198.454346f, 477.720581f } },
//        { "Throne of the Four Winds (PMV)",{ 1, -11355.091797f, 57.796021f, 723.884033f } },
//        { "The Top of the Tornado",{ 1, -11512.080078f, -2309.046143f, 608.393677f } },
//        { "Twilight Time",{ 1, -8292.992188f, -4574.514648f, -225.006012f } },
//        { "Grim Batol",{ 0, -4063.872559f, -3451.425537f, 280.219177f } },
//        { "The Lost City of Tol'vir",{ 1, -10672.040039f, -1304.768555f, 15.353432f } },
//        { "Sul'Aman",{ 530, 6851.040527f, -7994.026855f, 192.370026f } },
//        { "Sul'Gurub",{ 0, -11916.274414f, -1209.298706f, 92.288185f } },
//        { "Source of Eternity",{ 1, -8598.936523f, -4013.136719f, -205.547363f } },
//        { "Stone Nedra",{ 646, 1023.826843f, 641.982727f, 156.672241f } },
//        { "The End of Time",{ 1, -8300.198242f, -4460.448242f, -208.361374f } },
//        { "Dark Fang Fortress",{ 0, -245.108505f, 1535.278564f, 76.884575f } },
//        { "Dead Copies",{ 0, -11208.404297f, 1673.227905f, 24.640856f } },
//        { "Caves of the Black Mountain",{ 0, -7571.501953f, -1323.749756f, 245.536789f } },
//        { "Throne of Tides",{ 0, -5597.484863f, 5410.770020f, -1798.624512f } },
//        { "The Stars of Creation",{ 1, -10183.499023f, -2005.540039f, 54.850933f } },
//        { "Fire Advanced",{ 861, 1026.512939f, 326.450012f, 43.128017f } },
//        { "Tron Terathan",{ 646, 2356.175537f, 168.990829f, 181.397308f } } } },
//
//    // Northrend:
//    { 3,{ { "The Citadel of the Ice Crown (CLK)",{ 571, 5798.973145f, 2075.413330f, 636.064270f } },
//        { "Ulduar",{ 571, 9329.967773f, -1117.265991f, 1245.147095f } },
//        { "Naxramas",{ 571, 3681.921387f, -1269.339722f, 243.550156f } },
//        { "The Crusader's Test (IK)",{ 571, 8515.189453f, 730.540405f, 558.248169f } },
//        { "Ruby Sanctuary (RS)",{ 571, 3599.100342f, 198.728455f, -113.918884f } },
//        { "Onyxia's Lair",{ 1, -4706.007324f, -3725.238037f, 54.216221f } },
//        { "Azjol-Nerub",{ 571, 3677.481445f, 2166.978760f, 35.823750f } },
//        { "Top of Utgard",{ 571, 1242.485474f, -4857.526367f, 217.807739f } },
//        { "Gundrak",{ 571, 6937.031250f, -4436.122070f, 450.518768f } },
//        { "Champion's Test",{ 571, 8575.139648f, 792.034912f, 558.514832f } },
//        { "Fortress of Drak'Taron",{ 571, 4782.254395f, -2045.779663f, 229.431503f } },
//        { "Utgard Fortress",{ 571, 1224.328735f, -4863.303223f, 41.248013f } },
//        { "The Forge of Souls",{ 571, 5663.309570f, 2012.766235f, 798.042419f } },
//        { "Nexus",{ 571, 3891.989014f, 6985.246094f, 69.488075f } },
//        { "The Purification of Stratholme",{ 1, -8755.321289f, -4450.678223f, -199.746170f } },
//        { "Lightning Hearts",{ 571, 9183.208984f, -1384.422485f, 1110.215332f } },
//        { "Saron's Pit",{ 571, 5600.267578f, 2017.103882f, 798.042358f } },
//        { "Forest of Crystal Songs",{ 571, 5454.924805f, 79.064369f, 176.894958f } },
//        { "Dun Niffel",{ 571, 7348.333008f, -2605.087891f, 814.849304f } },
//        { "Camp D.E.G.O.Y.",{ 571, 3228.040527f, 5279.141602f, 46.991146f } },
//        { "Lake Ice Shackles (OLO)",{ 571, 4603.230469f, 2846.640381f, 396.896454f } },
//        { "Camp Hrothgara",{ 571, 9920.071289f, 1139.714355f, 58.439980f } } } },
//
//    // Outland:
//    { 2,{ {"Black Temple (CH)",{ 530, -3642.185791f, 315.732849f, 35.080578f } },
//        { "Plateau of the Sunwell (PSC)",{ 530, 12560.092773f, -6774.750488f, 15.031774f } },
//        { "Magtherideon's Lair",{ 530, -314.911316f, 3089.671875f, -116.477043f } },
//        { "Snake Sanctuary",{ 530, 795.544312f, 6864.642090f, -64.882057f } },
//        { "Fortress of Storms (KB)",{ 530, 3088.303467f, 1383.490479f, 184.822083f } },
//        { "Karajan",{ 0, -11119.043945f, -2010.794067f, 47.081917f } },
//        { "Gruula's Lair",{ 530, 3538.100586f, 5095.592773f, 3.827646f } },
//        { "Battle for Mount Hijal",{ 1, -8174.143555f, -4176.042969f, -166.160446f } },
//        { "Arcatraz",{ 530, 3308.318115f, 1341.447876f, 505.559998f } },
//        { "Aukenai Tombs",{ 530, -3361.969238f, 5218.605957f, -101.048927f } },
//        { "Bastions of Hell's Flame",{ 530, -362.368958f, 3075.630127f, -15.044868f } },
//        { "Botany",{ 530, 3404.688721f, 1490.116821f, 182.837402f } },
//        { "Mana's Tombs",{ 530, -3083.988770f, 4942.971680f, -101.047379f } },
//        { "Blood Forge",{ 530, -300.034454f, 3159.795898f, 31.686928f } },
//        { "Mechanar",{ 530, 2872.008789f, 1554.671753f, 252.159927f } },
//        { "Belowtope",{ 530, 781.553528f, 6757.112793f, -72.538170f } },
//        { "Steam Dungeon",{ 530, 817.702881f, 6936.059570f, -80.626816f } },
//        { "Destroyed Halls",{ 530, -309.308777f, 3076.211670f, -3.646897f } },
//        { "Setekk Halls",{ 530, -3361.872803f, 4665.686035f, -101.046959f } },
//        { "Old Hillsbrad",{ 1, -8364.833984f, -4058.000977f, -208.264832f } },
//        { "Dark Labyrinth",{ 530, -3635.138672f, 4943.125000f, -101.049004f } },
//        { "The Terrace of the Masters",{ 530, 12885.387695f, -7334.700195f, 65.487663f } },
//        { "The Inse",{ 530, 724.600220f, 7011.446777f, -72.262093f } },
//        { "Black Topi",{ 1, -8751.717773f, -4195.399414f, -209.498672f } },
//        { "Camp Dragon Paste",{ 530, -5109.739258f, 603.126282f, 85.105316f } },
//        { "Wayne's Shelter",{ 530, -2389.543213f, 2902.149414f, -55.578075f } },
//        { "Black Wind Camp",{ 530, -3365.530762f, 3649.803467f, 284.591309f } },
//        { "Halaa",{ 530, -1571.396729f, 7957.963867f, -22.601425f } },
//        { "Sporeggar",{ 530, 243.163574f, 8505.202148f, 22.840126f } },
//        { "Kenari operfrom",{ 530, -189.839386f, 5518.477539f, 28.825733f } },
//        { "Ogri'la",{ 530, 2323.647949f, 7282.838379f, 365.618591f } },
//        { "Kirin'var Village",{ 530, 2264.510742f, 2271.068848f, 97.540649f } },
//        { "Throne of Kil'Djedin",{ 530, 909.243591f, 2269.343994f, 310.259460f } } } },
//
//    // Kalimdor and the Eastern Kingdoms :
//        { 1,{ { "Fire Bowels (ON)",{ 0, -7511.410645f, -1042.036011f, 180.911377f } },
//            { "The Lair of the Wing of Darkness (LKT)",{ 0, -7659.681641f, -1220.593994f, 287.789124f } },
//            { "Ruins of An'Kiraj",{ 1, -8415.337891f, 1505.528198f, 31.278450f } },
//            { "Temple of An'Kiraj",{ 1, -8237.951172f, 1990.962036f, 129.072693f } },
//            { "Depths of the Black Mountain",{ 0, -7179.401367f, -922.581665f, 166.169739f } },
//            { "Gnomregan",{ 0, -5143.506348f, 895.267761f, 287.394073f } },
//            { "The Forgotten City",{ 1, -3789.423340f, 1242.360352f, 160.269928f } },
//            { "Halls of the Scarlet Order",{ 0, 2872.347168f, -819.494568f, 160.332001f } },
//            { "Sul'Farrak",{ 1, -6796.538086f, -2890.864746f, 8.881607f } },
//            { "Dark Fang Fortress",{ 0, -245.108505f, 1535.278564f, 76.884575f } },
//            { "Mounds of Igloshkuri",{ 1, -4658.570312f, -2525.889648f, 81.444489f } },
//            { "Labyrinths of the Igloskins",{ 1, -4466.629883f, -1669.908203f, 81.892502f } },
//            { "Marrodon",{ 1, -1187.513550f, 2879.690430f, 85.815750f } },
//            { "Dead Copies",{ 0, -11208.404297f, 1673.227905f, 24.640856f } },
//            { "Monastery of the Scarlet Order",{ 0, 2913.326416f, -802.447327f, 160.332001f } },
//            { "Necrosite",{ 0, 1257.583252f, -2584.631348f, 92.887039f } },
//            { "The Impenetrable Abyss",{ 1, 4248.132812f, 743.609131f, -24.830606f } },
//            { "Lower Part of the Peak of the Black Mountain",{ 0, -7526.470703f, -1226.190918f, 285.732758f } },
//            { "Fire Gap",{ 1, 1813.999023f, -4416.091309f, -18.220598f } },
//            { "Caves of The Wall",{ 1, -741.325806f, -2215.993164f, 15.928796f } },
//            { "Stratholme",{ 0, 3356.535400f, -3379.539062f, 144.782700f } },
//            { "Stratholme (back entrance)",{ 0, 3177.213379f, -4038.888916f, 105.441299f } },
//            { "Prison",{ 0, -8786.644531f, 829.226624f, 97.650917f } },
//            { "Uldaman",{ 0, -6070.387207f, -2955.664795f, 209.780655f } },
//            { "Temple of Atal'Hakkara",{ 0, -10291.303711f, -3985.823730f, -70.735390f } },
//            { "Underground Train",{ 369, -19.843245f, 37.441551f, -4.297364f } },
//            { "Caves of Time",{ 1, -8445.805664f, -4362.279297f, -176.302505f } },
//            { "Dalaran Crater",{ 0, 310.012177f, 294.056641f, 80.379440f } },
//            { "The Manor of the Black Crow",{ 0, 23.520702f, -1603.598022f, 195.420502f } },
//            { "Talassian Pass",{ 0, 3465.497314f, -4476.250000f, 137.350983f } },
//            { "Moonlight",{ 1, 7792.039062f, -2405.552490f, 489.165863f } } }  },
//};
//
//class npc_teleoprter : public CreatureScript
//{
//public:
//	npc_teleoprter() : CreatureScript("npc_teleoprter") { }
//
//    uint32 PriceInGold = 5000 *10000; // 5000 golds
//
//	bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//	{
//        pPlayer->ADD_GOSSIP_ITEM(5, "Kalimdor and the Eastern Kingdoms", GOSSIP_SENDER_SEC_STABLEMASTER, 1);
//        pPlayer->ADD_GOSSIP_ITEM(5, "Outland", GOSSIP_SENDER_SEC_STABLEMASTER, 2);
//        pPlayer->ADD_GOSSIP_ITEM(5, "Northrend", GOSSIP_SENDER_SEC_STABLEMASTER, 3);
//        pPlayer->ADD_GOSSIP_ITEM(5, "Cataclysm", GOSSIP_SENDER_SEC_STABLEMASTER, 4);
//        pPlayer->ADD_GOSSIP_ITEM(5, "Pandaria", GOSSIP_SENDER_SEC_STABLEMASTER, 5);
//        pPlayer->ADD_GOSSIP_ITEM(5, "Arena", GOSSIP_SENDER_SEC_STABLEMASTER, 6);
//		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
//		return true;
//	}
//
//	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
//	{
//        player->PlayerTalkClass->SendCloseGossip();
//        if (sender == GOSSIP_SENDER_SEC_STABLEMASTER)
//        {
//            for (uint32 i =0; i < teleMenu[action].size(); ++i)
//            {
//                auto inf = teleMenu[action][i];
//                player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_MONEY_BAG, inf.text.c_str(), action, i+1, "", PriceInGold, false);
//            }
//            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
//            return true;
//        }
//		
//        if (!player->HasEnoughMoney(uint64(PriceInGold)))
//            player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, NULL, 0, 0);
//        else if (teleMenu[sender].size() > (action - 1))
//        {
//            player->TeleportTo(teleMenu[sender][action - 1].loc);
//            player->ModifyMoney(-1*PriceInGold);
//        }
//        player->CLOSE_GOSSIP_MENU();
//        return true;
//	}
//};

void AddSC_teleguy()
{
    new Teleguy();
	//new npc_teleoprter();
}

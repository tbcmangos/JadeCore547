/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Transport.h"
#include "GridDefines.h"
#include "MapInstanced.h"
#include "InstanceScript.h"
#include "Config.h"
#include "World.h"
#include "CellImpl.h"
#include "Corpse.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "WorldPacket.h"
#include "Group.h"
#include <thread>
//npcbot
#include "botmgr.h"
#define Trinity UwowCore
//end npcbot

MapManager::MapManager()
{
    i_gridCleanUpDelay = sWorld->getIntConfig(CONFIG_INTERVAL_GRIDCLEAN);
    i_timer.SetInterval(sWorld->getIntConfig(CONFIG_INTERVAL_MAPUPDATE));
}

MapManager::~MapManager()
{
}

void MapManager::Initialize()
{
    Map::InitStateMachine();

    int num_threads(sWorld->getIntConfig(CONFIG_NUMTHREADS));
    // Start mtmaps if needed.
    if (num_threads > 0)
        m_updater.activate(num_threads);

    //npcbot - spawn bots
    BotMgr::LoadConfig();

    if (!BotMgr::IsNpcBotModEnabled())
        return;

    uint32 botoldMSTime = getMSTime();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Starting NpcBot system...");
    PreparedStatement* botstmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_NPCBOTS);
    //"SELECT entry FROM characters_npcbot", CONNECTION_SYNCH
    PreparedQueryResult res = CharacterDatabase.Query(botstmt);
    if (!res)
    {
		sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Spawned 0 npcbots. Table `characters_npcbot` is empty!");
        return;
    }

    PreparedQueryResult infores;
    uint32 botcounter = 0;
    Field* field;
    std::list<uint32> botgrids;
    do
    {
        field = res->Fetch();
        uint32 entry = field[0].GetUInt32();
        CreatureTemplate const* proto = sObjectMgr->GetCreatureTemplate(entry);
        if (!proto)
        {
			sLog->outError(LOG_FILTER_SERVER_LOADING, "Cannot find creature_template entry for npcbot (id: %u)!", entry);
            continue;
        }

        botstmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_NPCBOT_INFO);
        //"SELECT guid, map, position_x, position_y, position_z, orientation FROM creature WHERE id = ?", CONNECTION_SYNCH
        botstmt->setUInt32(0, entry);
        infores = WorldDatabase.Query(botstmt);
        if (!infores)
        {
			sLog->outError(LOG_FILTER_SERVER_LOADING, "Cannot spawn npcbot %s (id: %u), not found in `creature` table!", proto->Name.c_str(), entry);
            continue;
        }

        field = infores->Fetch();
        uint32 tableGuid = field[0].GetUInt32();
        uint32 mapId = uint32(field[1].GetUInt16());
        float pos_x = field[2].GetFloat();
        float pos_y = field[3].GetFloat();
        //float pos_z = field[4].GetFloat();
        //float ori = field[5].GetFloat();

        CellCoord c = UwowCore::ComputeCellCoord(pos_x, pos_y);
        GridCoord g = UwowCore::ComputeGridCoord(pos_x, pos_y);
        ASSERT(c.IsCoordValid() && "Invalid Cell coord!");
        ASSERT(g.IsCoordValid() && "Invalid Grid coord!");
        Map* npcbotmap = sMapMgr->CreateBaseMap(mapId);
        npcbotmap->LoadGrid(pos_x, pos_y);
        /*Creature* bot = npcbotmap->GetCreature(ObjectGuid(HighGuid::Unit, entry, tableGuid));
        ABORT();
        //debug
        if (!bot->IsAlive())
        {
            bot->Respawn();
            bot->ResetBotAI(1);
        }*/

		sLog->outDebug(LOG_FILTER_SERVER_LOADING, ">> Spawned npcbot %s (id: %u, map: %u, grid: %u, cell: %u)", proto->Name.c_str(), entry, mapId, g.GetId(), c.GetId());
        botgrids.push_back(g.GetId());
        ++botcounter;

    } while (res->NextRow());

    botgrids.unique();
	sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Spawned %u npcbot(s) within %lu grid(s) in %u ms", botcounter, botgrids.size(), GetMSTimeDiffToNow(botoldMSTime));
    //end npcbot
}

void MapManager::InitializeVisibilityDistanceInfo()
{
    for (MapMapType::iterator iter=i_maps.begin(); iter != i_maps.end(); ++iter)
        (*iter).second->InitVisibilityDistance();
}

Map* MapManager::CreateBaseMap(uint32 id)
{
    Map* map = FindBaseMap(id);

    if (map == NULL)
    {
        TRINITY_GUARD(ACE_Thread_Mutex, Lock);

        MapEntry const* entry = sMapStore.LookupEntry(id);
        ASSERT(entry);

        if (entry->Instanceable())
            map = new MapInstanced(id, i_gridCleanUpDelay);
        else
        {
            map = new Map(id, i_gridCleanUpDelay, 0, NONE_DIFFICULTY);
            map->LoadRespawnTimes();
            map->LoadCorpseData();
        }

        i_maps[id] = map;
    }

    ASSERT(map);
    return map;
}

Map* MapManager::FindBaseNonInstanceMap(uint32 mapId) const
{
    Map* map = FindBaseMap(mapId);
    if (map && map->Instanceable())
        return NULL;
    return map;
}

Map* MapManager::CreateMap(uint32 id, Player* player)
{
    Map* m = CreateBaseMap(id);

    if (m && m->Instanceable())
        m = ((MapInstanced*)m)->CreateInstanceForPlayer(id, player);

    return m;
}

Map* MapManager::FindMap(uint32 mapid, uint32 instanceId) const
{
    Map* map = FindBaseMap(mapid);
    if (!map)
        return NULL;

    if (!map->Instanceable())
        return instanceId == 0 ? map : NULL;

    return ((MapInstanced*)map)->FindInstanceMap(instanceId);
}

Map* MapManager::FindRaidMap(uint32 mapid, RaidEncounter* encounter) const
{
    Map* map = FindBaseMap(mapid);
    if (!map)
        return NULL;

    if (!map->IsRaid())
        return encounter ? NULL : map;

    return ((MapInstanced*)map)->FindRaidEncounterMap(encounter);
}

bool MapManager::CanPlayerEnter(uint32 mapid, Player* player, bool loginCheck)
{
    MapEntry const* entry = sMapStore.LookupEntry(mapid);
    if (!entry)
       return false;

    // Molten Front
    // That's not an instance, so check it here
    if (entry->MapID == 861)
        if (player->getLevel() < 85)
            return false;

    if (!entry->IsDungeon())
        return true;

    InstanceTemplate const* instance = sObjectMgr->GetInstanceTemplate(mapid);
    if (!instance)
        return false;

    Difficulty targetDifficulty = player->GetDifficulty(entry->IsRaid());
    //The player has a heroic mode and tries to enter into instance which has no a heroic mode
    MapDifficulty const* mapDiff = GetMapDifficultyData(entry->MapID, targetDifficulty);
    if (!mapDiff)
    {
        // Send aborted message for dungeons
        if (entry->IsNonRaidDungeon())
        {
            player->SendTransferAborted(mapid, TRANSFER_ABORT_NONE, player->GetDungeonDifficulty());
            return false;
        }
        else    // attempt to downscale
            mapDiff = GetDownscaledMapDifficultyData(entry->MapID, targetDifficulty);
    }

    //Bypass checks for GMs
    if (player->isGameMaster())
        return true;

    char const* mapName = entry->name;

    if (!player->isAlive())
    {
        if (player->HasCorpse())
        {
            // let enter in ghost mode in instance that connected to inner instance with corpse
            uint32 corpseMap = player->GetCorpseLocation().GetMapId();
            do
            {
                if (corpseMap == mapid)
                    break;

                InstanceTemplate const* corpseInstance = sObjectMgr->GetInstanceTemplate(corpseMap);
                corpseMap = corpseInstance ? corpseInstance->Parent : 0;
            }
            while (corpseMap);

            if (!corpseMap)
            {
                WorldPacket data(SMSG_CORPSE_NOT_IN_INSTANCE, 0);
                player->GetSession()->SendPacket(&data);
                sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' does not have a corpse in instance '%s' and cannot enter.", player->GetName(), mapName);
                return false;
            }
            sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' has corpse in instance '%s' and can enter.", player->GetName(), mapName);
        }
        else
            sLog->outDebug(LOG_FILTER_MAPS, "Map::CanPlayerEnter - player '%s' is dead but does not have a corpse!", player->GetName());
    }
    
    Group* group = player->GetGroup();
    if (entry->IsRaid() && entry->Expansion() >= 4)
    {
        // can only enter in a raid group
        if ((!group || !group->isRaidGroup()) && !sWorld->getBoolConfig(CONFIG_INSTANCE_IGNORE_RAID))
        {
            // probably there must be special opcode, because client has this string constant in GlobalStrings.lua
            // TODO: this is not a good place to send the message
            player->GetSession()->SendAreaTriggerMessage(player->GetSession()->GetTrinityString(LANG_INSTANCE_RAID_GROUP_ONLY), mapName);
            sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' must be in a raid group to enter instance '%s'", player->GetName(), mapName);
            return false;
        }
    }

    //Get instance where player's group is bound & its map
    if (group)
    {
        InstanceGroupBind* boundInstance = group->GetBoundInstance(entry);
        if (boundInstance && boundInstance->save)
            if (Map* boundMap = sMapMgr->FindMap(mapid, boundInstance->save->GetInstanceId()))
                if (!loginCheck && !boundMap->CanEnter(player))
                    return false;
            /*
                This check has to be moved to InstanceMap::CanEnter()
                // Player permanently bounded to different instance than groups one
                InstancePlayerBind* playerBoundedInstance = player->GetBoundInstance(mapid, player->GetDifficulty(entry->IsRaid()));
                if (playerBoundedInstance && playerBoundedInstance->perm && playerBoundedInstance->save &&
                    boundedInstance->save->GetInstanceId() != playerBoundedInstance->save->GetInstanceId())
                {
                    //TODO: send some kind of error message to the player
                    return false;
                }*/

        if (!group->CanEnterInInstance())
            return false;
    }

    // players are only allowed to enter 5 instances per hour
    if (entry->IsDungeon() && (!player->GetGroup() || (player->GetGroup() && !player->GetGroup()->isLFGGroup())))
    {
        uint32 instaceIdToCheck = 0;
        if (InstanceSave* save = player->GetInstanceSave(mapid, entry->IsRaid()))
            instaceIdToCheck = save->GetInstanceId();

        // instanceId can never be 0 - will not be found
        if (!player->CheckInstanceCount(instaceIdToCheck) && !player->isDead())
        {
            player->SendTransferAborted(mapid, TRANSFER_ABORT_TOO_MANY_INSTANCES);
            return false;
        }
    }

    //Other requirements
    return player->Satisfy(sObjectMgr->GetAccessRequirement(mapid, targetDifficulty), mapid, true);
}

void MapManager::Update(uint32 diff)
{
    i_timer.Update(diff);
    if (!i_timer.Passed())
        return;

    /// - Start Achievement criteria update processing thread
    sAchievementMgr->PrepareCriteriaUpdateTaskThread();

    for (auto l_PlayerTask : sAchievementMgr->GetPlayersCriteriaTask())
    {
        if (m_updater.activated())
            m_updater.schedule_specific(new AchievementCriteriaUpdateRequest(&m_updater, l_PlayerTask.second));
        else
        {
            /// Process all task in synchrone way
            auto l_Task = new AchievementCriteriaUpdateRequest(nullptr, l_PlayerTask.second);
            l_Task->call();
            delete l_Task;
        }
    }

    /// - Start map updater threads
    MapMapType::iterator iter = i_maps.begin();
    for (; iter != i_maps.end(); ++iter)
    {
        if (m_updater.activated())
            m_updater.schedule_update(*iter->second, uint32(i_timer.GetCurrent()));
        else
            iter->second->Update(uint32(i_timer.GetCurrent()));
    }

    if (m_updater.activated())
        m_updater.wait();

    sAchievementMgr->ClearPlayersCriteriaTask();

    for (iter = i_maps.begin(); iter != i_maps.end(); ++iter)
        iter->second->DelayedUpdate(uint32(i_timer.GetCurrent()));

    i_timer.SetCurrent(0);
}

void MapManager::DoDelayedMovesAndRemoves()
{
}

bool MapManager::ExistMapAndVMap(uint32 mapid, float x, float y)
{
    GridCoord p = UwowCore::ComputeGridCoord(x, y);

    int gx=63-p.x_coord;
    int gy=63-p.y_coord;

    return Map::ExistMap(mapid, gx, gy) && Map::ExistVMap(mapid, gx, gy);
}

bool MapManager::IsValidMAP(uint32 mapid, bool startUp)
{
    MapEntry const* mEntry = sMapStore.LookupEntry(mapid);

    if (startUp)
        return mEntry ? true : false;
    else
        return mEntry && (!mEntry->IsDungeon() || sObjectMgr->GetInstanceTemplate(mapid));

    // TODO: add check for battleground template
}

void MapManager::UnloadAll()
{

    for (MapMapType::iterator iter = i_maps.begin(); iter != i_maps.end();)
    {
        iter->second->UnloadAll();
        delete iter->second;
        i_maps.erase(iter++);
    }

    if (m_updater.activated())
        m_updater.deactivate();

    Map::DeleteStateMachine();
}

uint32 MapManager::GetNumInstances()
{
    TRINITY_GUARD(ACE_Thread_Mutex, Lock);

    uint32 ret = 0;
    for (MapMapType::iterator itr = i_maps.begin(); itr != i_maps.end(); ++itr)
    {
        Map* map = itr->second;
        if (!map->Instanceable())
            continue;
        MapInstanced::InstancedMaps &maps = ((MapInstanced*)map)->GetInstancedMaps();
        for (MapInstanced::InstancedMaps::iterator mitr = maps.begin(); mitr != maps.end(); ++mitr)
            if (mitr->second->IsDungeon()) ret++;
    }
    return ret;
}

uint32 MapManager::GetNumPlayersInInstances()
{
    TRINITY_GUARD(ACE_Thread_Mutex, Lock);

    uint32 ret = 0;
    for (MapMapType::iterator itr = i_maps.begin(); itr != i_maps.end(); ++itr)
    {
        Map* map = itr->second;
        if (!map->Instanceable())
            continue;
        MapInstanced::InstancedMaps &maps = ((MapInstanced*)map)->GetInstancedMaps();
        for (MapInstanced::InstancedMaps::iterator mitr = maps.begin(); mitr != maps.end(); ++mitr)
            if (mitr->second->IsDungeon())
                ret += ((InstanceMap*)mitr->second)->GetPlayers().getSize();
    }
    return ret;
}

void MapManager::InitInstanceIds()
{
    _nextInstanceId = 1;

    QueryResult result = CharacterDatabase.Query("SELECT MAX(id) FROM instance");
    if (result)
    {
        uint32 maxId = (*result)[0].GetUInt32();

        // Resize to multiples of 32 (vector<bool> allocates memory the same way)
        _instanceIds.resize((maxId / 32) * 32 + (maxId % 32 > 0 ? 32 : 0));
    }
}

void MapManager::RegisterInstanceId(uint32 instanceId)
{
    // Allocation and sizing was done in InitInstanceIds()
    _instanceIds[instanceId] = true;
}

uint32 MapManager::GenerateInstanceId()
{
    uint32 newInstanceId = _nextInstanceId;

    // Find the lowest available id starting from the current NextInstanceId (which should be the lowest according to the logic in FreeInstanceId()
    for (uint32 i = ++_nextInstanceId; i < 0xFFFFFFFF; ++i)
    {
        if ((i < _instanceIds.size() && !_instanceIds[i]) || i >= _instanceIds.size())
        {
            _nextInstanceId = i;
            break;
        }
    }

    if (newInstanceId == _nextInstanceId)
    {
        sLog->outError(LOG_FILTER_MAPS, "Instance ID overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }

    // Allocate space if necessary
    if (newInstanceId >= uint32(_instanceIds.size()))
    {
        // Due to the odd memory allocation behavior of vector<bool> we match size to capacity before triggering a new allocation
        if (_instanceIds.size() < _instanceIds.capacity())
        {
            _instanceIds.resize(_instanceIds.capacity());
        }
        else
            _instanceIds.resize((newInstanceId / 32) * 32 + (newInstanceId % 32 > 0 ? 32 : 0));
    }

    _instanceIds[newInstanceId] = true;

    return newInstanceId;
}

void MapManager::FreeInstanceId(uint32 instanceId)
{
    // If freed instance id is lower than the next id available for new instances, use the freed one instead
    if (instanceId < _nextInstanceId)
        SetNextInstanceId(instanceId);

    _instanceIds[instanceId] = false;
}
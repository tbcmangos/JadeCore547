/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50562
Source Host           : localhost:3306
Source Database       : world2

Target Server Type    : MYSQL
Target Server Version : 50562
File Encoding         : 65001

Date: 2020-02-11 16:33:04
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for npc_areaguard
-- ----------------------------
DROP TABLE IF EXISTS `npc_areaguard`;
CREATE TABLE `npc_areaguard` (
  `creature_entry` mediumint(8) unsigned NOT NULL,
  `guard_type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=All,1=Team,2=SecLvl,3=PlrLvl,4=Guild,5=PlrGuid,6=requirequestid',
  `trigger_distance` float NOT NULL DEFAULT '65' COMMENT 'In Feet - Max is 65',
  `req_value` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'For Team 1=Ally,2=Horde',
  `tele_map` mediumint(8) unsigned NOT NULL DEFAULT '1' COMMENT 'Dflt tele dest is Ratchet',
  `tele_x` float NOT NULL DEFAULT '-955.726',
  `tele_y` float NOT NULL DEFAULT '-3747.16',
  `tele_z` float NOT NULL DEFAULT '5.47817',
  `tele_o` float NOT NULL DEFAULT '1.6369',
  `comment` varchar(40) NOT NULL,
  PRIMARY KEY (`creature_entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC Area Guard';

-- ----------------------------
-- Records of npc_areaguard
-- ----------------------------
INSERT INTO `npc_areaguard` VALUES ('900010', '0', '65', '0', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走非GM和GM未开的玩家');
INSERT INTO `npc_areaguard` VALUES ('900011', '1', '65', '1', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走非联盟玩家');
INSERT INTO `npc_areaguard` VALUES ('900012', '2', '65', '2', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走非部落玩家');
INSERT INTO `npc_areaguard` VALUES ('900013', '3', '65', '1', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走GM等级小于1的玩家玩家');
INSERT INTO `npc_areaguard` VALUES ('900014', '4', '65', '10', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走游戏等级小于10的玩家');
INSERT INTO `npc_areaguard` VALUES ('900015', '5', '65', '2', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走GUID不等于2的玩家');
INSERT INTO `npc_areaguard` VALUES ('900016', '6', '65', '0', '1', '-955.726', '-3747.16', '5.47817', '1.6369', '传送走没完成某个任务的玩家');


delete from creature_template where entry between 900000 and 900003;

INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `difficulty_entry_11`, `difficulty_entry_12`, `difficulty_entry_13`, `difficulty_entry_14`, `difficulty_entry_15`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES 
('900010', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '管理员区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900011', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '联盟区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900012', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '部落区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900013', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', 'GM等级区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900014', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '玩家等级区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900015', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '玩家GUID区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048'),
('900016', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '27587', '0', '0', '0', '任务完成区域守护', '区域守护大师', '', '10025', '80', '80', '0', '0', '35', '35', '3', '0', '1', '1.14286', '1.14286', '1', '0', '422', '586', '0', '642', '1', '2000', '2000', '1', '0', '2048', '0', '0', '0', '0', '0', '0', '345', '509', '103', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '1', '31081', '0', '0', 'npc_areaguard', '16048');
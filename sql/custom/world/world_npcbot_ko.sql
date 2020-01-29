SET @BOT_START = 90000;
SET @BOT_END   = 91000;

UPDATE `creature_template` SET `subname` = '战士机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 1;
UPDATE `creature_template` SET `subname` = '骑士机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 2;
UPDATE `creature_template` SET `subname` = '猎人机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 3;
UPDATE `creature_template` SET `subname` = '盗贼机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 4;
UPDATE `creature_template` SET `subname` = '牧师机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 5;
UPDATE `creature_template` SET `subname` = '死骑机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 6;
UPDATE `creature_template` SET `subname` = '萨满机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 7;
UPDATE `creature_template` SET `subname` = '法师机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 8;
UPDATE `creature_template` SET `subname` = '术士机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 9;
UPDATE `creature_template` SET `subname` = '武僧机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 10;
UPDATE `creature_template` SET `subname` = '德鲁伊机器人' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 11;
UPDATE `creature_template` SET `subname` = '검신 봇' WHERE (`entry` BETWEEN @BOT_START AND @BOT_END) AND `trainer_class` = 12;


-- I live only to serve the master.
UPDATE `npc_text` SET `text0_0` = '我活着就是为了为主人服务' WHERE `ID` = @BOT_START+0001;
-- You need something?
UPDATE `npc_text` SET `text0_0` = '需要点什么?' WHERE `ID` = @BOT_START+0002;
-- Mortals... usually I kill wretches like you at sight
UPDATE `npc_text` SET `text0_0` = '凡人.. 通常我见到你就想杀了你.' WHERE `ID` = @BOT_START+0003;

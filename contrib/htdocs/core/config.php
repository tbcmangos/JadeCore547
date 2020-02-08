<?php
/*********************************************
*   PACW - Pingue Account Creation Website   *
*                 Version: 1.0               *
*            coded by Adam Vi餫rsson         *
*                                            *
*             http://thepingue.com           *
*        MODIFIED BY MYRAN2 OF MMOWNED       *
*********************************************/	
	header("Content-Type:text/html;charset=utf-8");
	// DATABASE CONNECTION
	$dbhost = 'localhost';
	$serverport = 8085;
	$dbuser = 'root';
	$dbpass = 'afaf65b113286597'; 
	$dbaccs = 'w_auth';
	$dbchars = 'w_char';

	
	
	//XML STATS FILE (ONLY ARCEMU)
	$xmlfile = 'stats.xml'; 	// You need to provide a link to your XML file for server status to work correctly.
	$xmldisplay = 0; 			/* 1 = Displays Uptime & Online GMs
                                   2 = Displays only Uptime
                                   3 = Displays only Online GMs 
	                               0 = Disables showing Uptime/Online GMs */
	
	// WEBSITE SETTINGS
	$multi_ip = 1; 				// 1 = Allows multi IP registration, 0 = Disables multi IP registration.
	$js = 0;			    	// 0 = Fade effect, 1 = Slide effect.
	$theme = 2;             	/* Theme selector:
									0 = Nightvision (Default)
									1 = Tbc VS WotLK 
									2 = Cataclysm
								*/
	$max_players = 100;         // Configures how many players can play on the realm at the same time.
	$show_players = 50;         /* Configures how many players are shown on the status page, picks out randoms.
	 						 	   Having this count very high may slow down the loading time. */
	$show_all = true;           // true = Enables "Show All Link", false = Disables "Show All Link" (Of players online).
	
	// HOW TO CONNECT - EDIT HERE.
	$realmlist = 'Set realmlist 39.105.210.193';
	$htc1t = '设置服务器列表.';
	$htc1 = '打开wow目录下的WTF\Config.wtf文件，将Set realmlist xxx 改为 Set realmlist 39.105.210.193<br />';
	
	$htc2t = '创建账户!';
	$htc2 = ' <a href="index.php">点击这里</a>';
	
	$htc3t = '等待';
	$htc3 = '等待一分钟左右. ';
	
	$htc4t = '启动WOW.exe即可登录!';
	$htc4 = '服务器设置：<br />
	出生1000G<br />
10倍掉落和经验<br />
0.1倍伤害<br />
3倍属性<br />
LM/BL互通<br />
10个商业技能<br />
每个人能招4个机器人<br/><br />';
?>
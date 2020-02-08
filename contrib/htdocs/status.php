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

$dbcon = @mysql_connect($dbhost, $dbuser, $dbpass);
if (!$dbcon)
{
  return show_error(mysql_error());
}

//port open
$realmstatus = '';
  if (! $sock = @fsockopen($dbhost, $serverport, $num, $error, 3))
    {
        $realmstatus = "<FONT COLOR=red>服务器离线</FONT>";
    }
    else
    { 
        $realmstatus = "<FONT COLOR=green>服务器在线</FONT>"; 
        fclose($sock);
    };

//uptime
$dba = @mysql_select_db($dbaccs, $dbcon);
if (!$dba)
{
  return show_error(mysql_error());
}

$qry = mysql_query("SELECT * FROM " . mysql_real_escape_string($dbaccs) . ".uptime ORDER BY `starttime` DESC LIMIT 1") or die(mysql_error());
$uptime_results = mysql_fetch_array($qry);    
$uptime = Sec2Time($uptime_results['uptime']);


//characters
$dbc = @mysql_select_db($dbchars, $dbcon);
if (!$dbc)
{
  return show_error(mysql_error());
}

$online = mysql_query("SELECT count(*) FROM characters WHERE online = '1'") or die(mysql_error());
$online_players = mysql_fetch_array($online);
$online_players = $online_players['count(*)'];

if($online_players < $max_players) {
$total = ($online_players * 100) / $max_players;
$replace = array("," => ".");
$string = $total;
}
else $total = 100;



?>
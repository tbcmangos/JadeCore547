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
	function show_error($error = "未知问题") {
	  echo '<div class="nosuccess"><span style="padding-left:25px;color:#FFF;font-weight:bold;">错误:</span> ' . $error . ' <div class="n-img"></div></div><br />';
	}
	function locked_error($error = "未知问题") {
	  echo '<div class="locked"><span style="padding-left:25px;color:#FFF;font-weight:bold;">错误:</span> ' . $error . ' <div class="l-img"></div></div><br />';
	}
	
	if($show_all) {
		if(isset($_GET['status']) && $_GET['status'] == "showall") {
			if(!file_exists('all.php'))
			return show_error("Show all page is missing, AAH, REDOWNLOAD NAO!");
			else include("all.php");
		}
	}
	
	function vTheme() {
		global $theme, $checker;
		switch ($theme) {
			case 0:
			echo 'nvision';
			break;
			case 1:
			echo 'tvw';
			break;
			case 2:
			echo 'cataclysm';
			break;
			default:
			echo 'nvision';
		}		
	}
		
	function str_replace_assoc($array,$string) {
		$from_array = array();
		$to_array = array();
		
		foreach ($array as $k => $v){
			$from_array[] = $k;
			$to_array[] = $v;
		}
		
		return str_replace($from_array,$to_array,$string);
	}
	
	function xmlStuff() {
		global $xmlfile, $xmldisplay;
		if($xmldisplay != 0) {
			if(file_exists($xmlfile)) {
				$xml = simplexml_load_file($xmlfile);
				
				if($xmldisplay != 2 && $xmldisplay != 3) {
					return "<div align='center'>Uptime: " . $xml->status->uptime . "</div><br /><div align='center'>Online GMs: " . $xml->status->gmcount . "</div><br />";
				}
				if($xmldisplay != 1 && $xmldisplay != 3) {
					return "<div align='center'>Uptime: " . $xml->status->uptime . "</div><br />";
				}
				if($xmldisplay != 2 && $xmldisplay != 1) {
					return "<div align='center'>Online GMs " . $xml->status->gmcount . "</div><br />";
				}
			} else {
				return show_error("Your xml file cannot be found, please insert right settings in config.php");
			}
		}	
	}
	
	function jStyle() {
      global $js;
      echo ($js == 1) ? 'slide' : 'fade';
    }
	
	//Mav approved ^^
	function displayPlayers() {
		global $show_players, $online_players, $show_all;
		if ($show_players < $online_players) echo '随机显示 ' . $show_players . ' / ' . $online_players . ' 玩家.';
        if($show_all) echo ' - <a href="#showall" onClick="getOnline(id);">显示所有</a>';
		else echo '显示中 ' . $online_players . ' 玩家.';
	}
	
	function is_valid_email ($email) {
		$qtext = '[^\\x0d\\x22\\x5c\\x80-\\xff]';
		$dtext = '[^\\x0d\\x5b-\\x5d\\x80-\\xff]';
		$atom = '[^\\x00-\\x20\\x22\\x28\\x29\\x2c\\x2e\\x3a-\\x3c'.
					'\\x3e\\x40\\x5b-\\x5d\\x7f-\\xff]+';
		$quoted_pair = '\\x5c\\x00-\\x7f';
		$domain_literal = "\\x5b($dtext|$quoted_pair)*\\x5d";
		$quoted_string = "\\x22($qtext|$quoted_pair)*\\x22";
		$domain_ref = $atom;
		$sub_domain = "($domain_ref|$domain_literal)";
		$word = "($atom|$quoted_string)";
		$domain = "$sub_domain(\\x2e$sub_domain)*";
		$local_part = "$word(\\x2e$word)*";
		$addr_spec = "$local_part\\x40$domain";
	 
		return preg_match("!^$addr_spec$!", $email) ? true : false;
	}
			
	function showPlayersOnline() {
		global $show_players;
		$poquery = mysql_query("SELECT name, race, class, gender, level FROM characters WHERE online = '1' ORDER BY RAND() LIMIT {$show_players}");
		if(mysql_num_rows($poquery) > 0) {
			while($row =mysql_fetch_array($poquery)) {
				echo '      <tr>
										<td width="185">'.$row['name'].'</td>
										<td width="71" align="center"><img src=\'images/stats/'.$row['race'].'-'.$row['gender'].'.gif\'></td>
										<td width="63" align="center"><img src=\'images/stats/'.$row['class'].'.gif\'></td>
										<td width="48" align="center">'.$row['level'].'</td>
									</tr>';
			}
		} else $error = '无玩家在线!';
		
	}
	
	function showAllPlayersOnline() {
		$poquery2 = mysql_query("SELECT name, race, class, gender, level FROM characters WHERE online = '1' ORDER BY RAND()") or die(mysql_error());
		if(mysql_num_rows($poquery2) > 0) {
			while($row =mysql_fetch_array($poquery2)) {
				echo '      <tr>
										<td width="185">'.$row['name'].'</td>
										<td width="71" align="center"><img src=\'images/stats/'.$row['race'].'-'.$row['gender'].'.gif\'></td>
										<td width="63" align="center"><img src=\'images/stats/'.$row['class'].'.gif\'></td>
										<td width="48" align="center">'.$row['level'].'</td>
									</tr>';
			}
		} else $error = '无玩家在线!';
	}
	
	function Sec2Time($time){
	    if(is_numeric($time)){
	    $value = array(
	      "years" => 0, "days" => 0, "hours" => 0,
	      "minutes" => 0, "seconds" => 0,
	    );
	    if($time >= 31556926){
	      $value["years"] = floor($time/31556926);
	      $time = ($time%31556926);
	    }
	    if($time >= 86400){
	      $value["days"] = floor($time/86400);
	      $time = ($time%86400);
	    }
	    if($time >= 3600){
	      $value["hours"] = floor($time/3600);
	      $time = ($time%3600);
	    }
	    if($time >= 60){
	      $value["minutes"] = floor($time/60);
	      $time = ($time%60);
	    }
	    $value["seconds"] = floor($time);
	    //return (array) $value;
	    //$t=$value["years"] ."年". $value["days"] ."天"." ". $value["hours"] ."小时". $value["minutes"] ."分".$value["seconds"]."秒";
		$t=  $value["days"] ."天"." ". $value["hours"] ."小时". $value["minutes"] ."分" ;
	    Return $t;
	    
	     }else{
	    return (bool) FALSE;
	    }
	}
	
?>
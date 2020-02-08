<?php 
/*********************************************
*   PACW - Pingue Account Creation Website   *
*                 Version: 1.0               *
*            coded by Adam Viðarsson         *
*                                            *
*             http://thepingue.com           *
*        MODIFIED BY MYRAN2 OF MMOWNED       *
*********************************************/	
header("Content-Type:text/html;charset=utf-8");
error_reporting(E_ALL);
include('core/config.php');
include('core/core.php');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>熊猫酒仙 5.4.8</title>
        <link href="style/main.css" rel="stylesheet" type="text/css" />
        <link href="style/theme/<?php echo vTheme();?>.css" rel="stylesheet" type="text/css" />
        <script type="text/javascript" src="js/jquery.js"></script>
		<script type="text/javascript" src="js/<?php echo jStyle();?>.js"></script>
		<script type="text/javascript">
			function getOnline(id) {
			$("#serverstats2").slideUp("medium");
			$.ajax({
			    url: 'js/online.php',
			    dataType: 'json',
			    async: true,
			    cache: false,
			    success: function (data) {
			  $("#serverstats2").html(data.online);
			  setTimeout(function() { $("#serverstats2").slideDown("slow"); }, 500);
			    }
			})
			}
		</script>
    </head>

    <body>
        <div id="logo"></div>
		<div id="content">
                        <div id="navi">
                            <a href="#first"   id="buttonone"><div class="link1"></div></a>
                            <a href="#second"  id="buttontwo"><div class="link2"></div></a>
                            <a href="#third"   id="buttonthree"><div class="link3"></div></a>
							<a href="https://pan.baidu.com/s/1f6F12cDp13PQaqHA8gwUlg/" id="buttonfive"><div class="link5"></div></a>
                        </div>
                <div id="first" class="con-bg">
                    	<?php include('create.php');?>
                </div>
                
				<?php include('status.php');?>
				
                <div id="second" class="con-bg">
                <h2 align="center"><?php echo $realmstatus ?></h2>
                <div class="sep"></div>
                		
                        <?php echo xmlStuff();?>
                        
                    <div id="serverstats">
                    	<?php echo $uptime;?>
                        <div id="counter-bg">
                            <div id="counter" style="width:<?php echo $string;?>%;"></div><br />
                            <?php echo $online_players . '/' . $max_players . ' 玩家在线';?>
                        </div>
                    </div><br />
                    <div id="serverstats2">
                    	<table width="387">
                        	<tr>
                            	<td width="185"  align="center"><h3>名称</h3></td>
                                <td width="71" align="center"><h3>种族</h3></td>
                                <td width="63" align="center"><h3>职业</h3></td>
                                <td width="48" align="center"><h3>级别</h3></td>
                            </tr>
                       	</table>
                        <div class="sep"></div>
                  		<table width="387">
                        	<?php echo showPlayersOnline();?>
                       	</table><br />
						<h3 align="center"><?php displayPlayers();?></h3>                    
                     </div>
                </div>
                
                <div id="third" class="con-bg">
                <?php echo "<h2>1. " . $htc1t . "</h2>";?>
                <?php echo "<p> " . $htc1 . "</p>";?>
                <h2 align="center"><?php echo $realmlist;?></h2><br />
                <?php echo "<h2>2. " . $htc2t . "</h2>";?>
                <?php echo "<p> " . $htc2 . "</p><br />";?>
                <?php echo "<h2>3. " . $htc3t . "</h2>";?>
                <?php echo "<p> " . $htc3 . "</p><br /><br />";?>
				<h1 align="center">4. <?php echo $htc4t;?></h1>
				<?php echo "<p> " . $htc4 . "</p><br /><br />";?>
                </div>
				
            </div>
            <div id="con-bot"></div>
    </div>
       <!-- <a href="http://thepingue.com"><div id="footer"></div></a>--> 
    </body>
</html>
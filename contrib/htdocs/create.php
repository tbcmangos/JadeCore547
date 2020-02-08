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
	$dba = @mysql_select_db($dbaccs, $dbcon);
	if (!$dba)
	{
	  return show_error(mysql_error());
	}

$ip = $_SERVER['REMOTE_ADDR'];

if($multi_ip != 1) {
	$query = @mysql_query("SELECT * FROM account WHERE last_ip = '$ip'") or die(mysql_error());
	if(mysql_num_rows($query) != 0) {
		return locked_error("You have already used this IP before to register an account.");
	} 
}

echo '<div id="acc">
<h2 align="center">创建账户</h2><br /><form method="post">账户名<br /><input type="text" name="accname" /><br /><br />密码<br /><input type="password" name="pword" /><br /><br />重复密码<br /><input type="password" name="pwordtwo" /><br /><br />电子邮件<br /><input type="text" name="accmail" /><br /><br /><input type="submit" value="" name="submit" /><br /><h1 align="center">第一次进入游戏请看完开场动画或者等待相同时间直到周围生物和物体加载完毕</h1><br /></form>
</div>';

if(isset($_POST['submit'])) {
  if(empty($_POST['accname']) || empty($_POST['pword']) || empty($_POST['pwordtwo']) || empty($_POST['accmail']))
    return show_error("请输入所有必要信息.");
$user = $_POST['accname'];
$pass = $_POST['pword'];
$email = $_POST['accmail'];
$hash = sha1(strtoupper($user).':'.strtoupper($pass));	

    if($_POST['pwordtwo'] != $_POST['pword']) return show_error("密码不符."); 
	if(!is_valid_email($_POST['accmail'])) return show_error("请输入有效电子邮箱");
	
    $query1 = @mysql_query("SELECT * FROM account WHERE username = '$_POST[accname]'") or die(mysql_error());
    if(mysql_num_rows($query1) != 0) return show_error("用户名已经存在.");
    
    $query2 = @mysql_query("SELECT * FROM account WHERE email = '$_POST[accmail]'") or die(mysql_error());
    if(mysql_num_rows($query2) != 0) return show_error("电子邮箱已经存在.");
    
    $query3 = mysql_query("INSERT INTO account (username, sha_pass_hash, email) VALUES ('".$user."','".$hash."','".$email."')") or die(mysql_error()); 
    if (mysql_affected_rows($dbcon) <= 0) return show_error("发生未知错误，请稍后重试!");

    echo '<div class="success"><span style="padding-left:35px;">创建成功!</span> <div class="s-img"></div></div><br />';
}

?>
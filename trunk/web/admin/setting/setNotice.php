<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

  <div id="tt">
    Set Notice
  </div>

  <div id="main">
  <form action="setNotice.php" method="post">
   <b>Last Notice:</b><br>
   <div id="last_notice"><?php echo get_notice(); ?></div>
   <br><br><br>
   <b>Set New Notice:</b><br>
   <input name="notice" size=120 maxlength=200/><br><br>
   <input type="submit", value="Submit" name="submit"/>&nbsp;&nbsp;<input type="reset" value="Reset">&nbsp;&nbsp;<input type="submit" value="Refresh">
  </div>

  <br />


  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>

<?php
	if (isset($_POST['submit'])){
		set_notice($_POST['notice']);
	}
?>

<?php
function get_notice()
{
	$header = "np00000000";

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
//	$tc->sendstr($problem_id)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0)
		$recv = $tc->recvstr($len);
	else
		$recv = null;
	$tc->close();
	return $recv;
}

function set_notice($notice)
{
	if (empty($notice)) return;
	$header = sprintf("%s%08d", "sn", strlen($notice));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($notice)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}
?>
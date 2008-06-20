<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}

	if (isset($_POST['submit'])){
		include('../common/tcpclient.php');
		if (!empty($_POST['nid']) && !empty($_POST['news'])) {
			update_news($_POST['nid'], $_POST['news']);
		}
		header('Location: setNews.php');
		exit;
	}
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
    Update News
  </div>

  <div id="main">
  <form action="udnews.php" method="post">
   <b>News ID: <?php echo $_GET['nid']; ?></b><br>
   <input type="hidden" name="nid" value="<?php echo $_GET['nid']; ?>">
   <b>Modify:</b><br>
   <input name="news" size=120 maxlength=200 value="<?php echo base64_decode($_GET['news']); ?>" /><br><br>
   <input type="submit", value="Submit" name="submit"/>&nbsp;&nbsp;<input type="reset" value="Reset">&nbsp;&nbsp;
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
function update_news($nid, $news)
{
	$d = "\001";
	$message = $nid.$d.$news.$d.' ';
	$header = sprintf("%s%08d", "un", strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}
?>
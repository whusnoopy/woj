<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'student'){
		header('Location: login.php');
		exit;
	}

	include('../common/tcpclient.php');
	$st = array();
	getPersonalInfo($_SESSION['user_id'], $st);
?>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Programming Management</title>
    <link href="../style/t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br><br>
	<b>Student</b><br>
	<hr><br>
	<b>Information</b><br><br>
	<table border=1>
	<tr><td>User ID</td><td><?php echo $st[0]; ?></td></tr>
	<tr><td>IC Card</td><td><?php echo $st[1]; ?></td></tr>
	<tr><td>Name</td><td><?php echo $st[2]; ?></td></tr>
	<tr><td>Grade</td><td><?php echo $st[4]; ?></td></tr>
	<tr><td>Avalible</td><td><?php echo $st[6]; ?></td></tr>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;
	</div>
</body>
</html>

<?php
function getPersonalInfo($uid, &$st)
{
//	$jl = array('1', '只有2个题','2008-05-28 12:00:00', 'C语言','2008', 'spring');
//	return;

	$header = sprintf("%s%08d", 'sp', strlen($uid));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($uid) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$st = explode("\001", $recv);
	}
	else
		$st = null;
	$tc->close();
	return;
}
?>
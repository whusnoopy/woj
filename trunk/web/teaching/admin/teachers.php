<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}

	include('../common/tcpclient.php');
	$tl = array();
	getTeacherList($tl);
?>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Programming Management</title>
    <link href="../style/t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br>
	<b>Teacher List</b><br>
	<hr><br>
	<table border=1>
	<th>User ID</th><th>Name</th><th>Available</th><th>Edit</th>
<?php
	$cnt = count($tl);
	for ($i=0; $i<$cnt; $i+=3)
		echo '<tr><td>'.$tl[$i].'</td><td>'.$tl[$i+1].'</td><td>'.$tl[$i+2].'</td><td><a href="udteacher.php?uid='.$tl[$i].'&name='.rawurlencode($tl[$i+1]).'">Edit</a></td></tr>';
?>
	</table>
	<br>
	<a href="index.php">Back</a>
	</div>
</body>
</html>

<?php
function getTeacherList(&$tl)
{

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr('tl00000000') or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$tl = explode("\001", $recv);
	}
	else
		$tl = null;
	$tc->close();
	return;
}

<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}

	if (isset($_GET['cid']))
		$cid = $_GET['cid'];
	else
		$cid = '-1';

	include('../common/tcpclient.php');
	$sl = array();
	getStudentList($cid, $sl);
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
	<b>Student List</b><br>
	<hr><br>
	<table border=1>
	<th>User ID</th><th>Name</th><th>ID Card</th><th>Available</th><th>Grade</th>
<?php
	$cnt = count($sl);
	for ($i=0; $i<$cnt; $i+=6){
		echo '<tr><td>'.$sl[$i].'</td><td>'.$sl[$i+1].'</td>';
		echo '<td>'.$sl[$i+2].'</td><td>'.$sl[$i+3].'</td><td>'.$sl[$i+4].'</td></tr>';
	}
?>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>
	</div>
</body>
</html>

<?php
function getStudentList($cid, &$sl)
{
//	$sl = array('200431500278', '刘潜', '421125198807055456', 'Y', '2004', '08');
//	return;

	$header = sprintf("%s%08d", 'sl', strlen($cid));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($cid) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$sl = explode("\001", $recv);
	}
	else
		$sl = null;
	$tc->close();
	return;
}

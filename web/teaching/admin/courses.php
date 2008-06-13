<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}
	include('../common/tcpclient.php');
	$cl = array();
	getCourseList($_SESSION['user_id'], $cl);
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
	<b>Course List</b><br>
	<hr><br>
	<table border=1>
	<th>Course ID</th><th>Description</th>
<?php
	$cnt = count($cl);
	for ($i=0; $i<$cnt; $i+=2)
		echo '<tr><td>'.$cl[$i].'</td>
		<td><a href="course.php?cid='.$cl[$i].'&desc='.$cl[$i+1].'">'.$cl[$i+1].'</a></td></tr>';
?>
	</table>
	<br>
	<a href="index.php">Back</a>&nbsp;&nbsp;
	<a href="addcourse.php">Add Course</a>
	</div>
</body>
</html>

<?php
function getCourseList($user_id, &$cl)
{
	$header = sprintf("%s%08d", 'ol', strlen($user_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($user_id) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$cl = explode("\001", $recv);
	}
	else
		$cl = null;
	$tc->close();
	return;
}

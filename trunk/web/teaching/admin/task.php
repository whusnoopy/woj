<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}
	if (isset($_GET['jid']))
		$jid = $_GET['jid'];
	else
		$jid = '-1';

	include('../common/tcpclient.php');
	$ti = array();
	getTaskInfo($jid, $ti);
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
	<b>Task <?php echo $jid?></b><br>
    <table border=1><th width="30%"></th><th></th>
<?php
		echo '<tr><td>Description</td><td>'.$ti[1].'</td></tr>';
		echo '<tr><td>Publish Time</td><td>'.$ti[2].'</td></tr>';
		echo '<tr><td>Course</td><td>'.$ti[3].'</td></tr>';
		echo '<tr><td>Year</td><td>'.$ti[4].'</td></tr>';
		echo '<tr><td>Term</td><td>'.($ti[5]=='S'?'Spring':'Fall').'</td></tr>';
		echo '<tr><td>Required Problems</td><td>';
		$num = $ti[6];
		$i = 7;
		for (; $i<7+$num; $i++)
			echo $ti[$i].'&nbsp;&nbsp;';
		echo '</td></tr>';
		echo '<tr><td>Optional Problems['.$ti[$i+2].']</td><td>';
		$num = $ti[$i+3];
		for ($j=$i+4; $j<$i+4+$num; $j++)
			echo $ti[$j].'&nbsp;&nbsp;';
		echo '</td></tr>';
?>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;
	</div>
</body>
</html>

<?php
function getTaskInfo($jid, &$ti)
{
//	$ti = array('1', '只有2个题','2008-05-28 12:00:00','2008', 'spring', '3', '1000', '1001', '1002', '1', '1', '2', '1004', '1005');
//	return;

	$header = sprintf("%s%08d", 'jp', strlen($jid));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($jid) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$ti = explode("\001", $recv);
	}
	else
		$ti = null;
	$tc->close();
	return;
}
?>
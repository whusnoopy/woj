<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'student'){
		header('Location: login.php');
		exit;
	}
	if (isset($_GET['jid']))
		$jid = $_GET['jid'];
	else
		$jid = '-1';

	include('../common/tcpclient.php');
	$sti = array();
	getStudentTaskInfo($_SESSION['user_id'], $jid, $sti);
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
	<b>Task <?php echo $jid?></b><br>
    <table border=1><th width="30%"></th><th></th>
<?php
		echo '<tr><td>Task ID</td><td>'.$sti[0].'</td></tr>';
		echo '<tr><td>Required Problems</td><td>';
		$num = $sti[1];
		$i = 2;
		for (; $i<2+$num*2; $i+=2){
			echo $sti[$i];
			if ($sti[$i+1] == 'Y')
				echo '<font color=green size=2>[Done]</font>';
			else
				echo '<font color=red size=2>[To do]</font>';
			echo '&nbsp;&nbsp;';
		}
		echo '</td></tr>';
		echo '<tr><td>Optional Problems['.$sti[$i+2].']</td><td>';
		$num = $sti[$i+3];
		for ($j=$i+4; $j<$i+4+$num*2; $j+=2){
			echo $sti[$j];
			if ($sti[$j+1] == 'Y')
				echo '<font color=green size=2>[Done]</font>';
			else
				echo '<font color=red size=2>[To do]</font>';
			echo '&nbsp;&nbsp;';
		}
		echo '</td></tr>';
?>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;
	</div>
</body>
</html>

<?php
function getStudentTaskInfo($uid, $jid, &$sti)
{
//	$ti = array('1', '只有2个题','2008-05-28 12:00:00','2008', 'spring', '3', '1000', '1001', '1002', '1', '1', '2', '1004', '1005');
//	return;
	$d = "\001";
	$header = sprintf("%s%08d", 'ij', strlen($uid.$d.$jid));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($uid.$d.$jid) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$sti = explode($d, $recv);
	}
	else
		$sti = null;
	$tc->close();
	return;
}
?>
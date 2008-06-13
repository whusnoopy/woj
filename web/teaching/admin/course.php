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
	$jl = array();
	getTaskList($cid, $jl);
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
	<b>Course <?php echo $cid?></b><br><hr width=500>
	<?php echo $_GET['desc']; ?>
	<br>
	<hr><br>
	<b>Task List</b><br><br>
	<table border=1>
	<th width="8%">Task ID</th>
	<th width="30%">Description</th>
	<th width="20%">Pubulish Time</th>
	<th width="18%">course</th>
	<th width="8%">Year</th>
	<th width="8%">Term</th>
	<th width="8%">Report</th>
<?php
	$cnt = count($jl);
	for ($i=0; $i<$cnt; $i+=6){
		echo '<tr><td>'.$jl[$i].'</td>';
		echo '<td><a href="task.php?jid='.$jl[$i].'&desc='.$jl[$i+1].'">'.$jl[$i+1].'</a></td>';
		echo '<td>'.$jl[$i+2].'</td><td>'.$jl[$i+3].'</td><td>'.$jl[$i+4].'</td><td>'.($jl[$i+5]=='S'?'Spring':'Fall').'</td>';
		echo '<td><a href="report.php?jid='.$jl[$i].'">Report</a></td></tr>';
	}
?>
	</table>
	<br>
	<a href="courses.php">Back</a>&nbsp;&nbsp;
	<a href="students.php?cid=<?php echo $cid;?>">Student List</a>&nbsp;&nbsp;
	<a href="arrange.php?cid=<?php echo $cid;?>">Add Task To This Course</a>
	</div>
</body>
</html>

<?php
function getTaskList($cid, &$jl)
{
//	$jl = array('1', '只有2个题','2008-05-28 12:00:00', 'C语言','2008', 'spring');
//	return;

	$header = sprintf("%s%08d", 'jl', strlen($cid));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($cid) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$jl = explode("\001", $recv);
	}
	else
		$jl = null;
	$tc->close();
	return;
}
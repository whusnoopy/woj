<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'teacher'){
		header('Location: login.php');
		exit;
	}
	if (isset($_GET['start']))
		$page = $_GET['start'];
	else
		$page = 0;
	if (isset($_GET['jid']))
		$jid = $_GET['jid'];
	else
		$jid = -1;
	include('../common/tcpclient.php');
	$stl = array();
	getStatusList($jid, $page, $stl);
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
	<b>Status</b><br>
	<hr><br>
	<table border=1>
	<th>Status ID</th>
	<th>User ID</th>
	<th>Problem ID</th>
	<th>Result</th>
	<th>Date Time</th>
	<th>DownLoad</th>
	<th>Judge</th>
<?php
	$cnt = count($stl);
	for ($i=0; $i<$cnt; $i+=11){
		echo '<tr><td>'.$stl[$i].'</td><td>'.$stl[$i+1].'</td><td>'.$stl[$i+2].'</td>';
		if ($stl[$i+3] == '1')
			echo '<td>Pass</td>';
		else
			echo '<td>No pass</td>';
		echo '<td>'.$stl[$i+5].'</td>';
		echo '<td><a href="down.php?sid='.$stl[$i].'">Download</a></td>';
		echo '<td><a href="judge.php?jid='.$jid.'&sid='.$stl[$i].'&pass=yes">pass</a> / <a href="judge.php?jid='.$jid.'&sid='.$stl[$i].'&pass=no">fail</a> </td></tr>';
	}

?>
	</table>
	<br>
<?php
	if ($page>0){
		$start= $page-1;
		echo '<a href="tstatus.php?jid='.$jid.'&start='.$start.'">Previous</a>&nbsp;&nbsp;';
	}
	if ($cnt==11*25){
		$start = $page+1;
		echo '<a href="tstatus.php?jid='.$jid.'&start='.$start.'">Previous</a>';
	}
?>
	</div>
</body>
</html>

<?php
function getStatusList($jid, $page, &$stl)
{
	$d = "\001";
	$header = sprintf("%s%08d", 'sa', strlen($jid.$d.$page));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($jid.$d.$page) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$stl = explode("\001", $recv);
	}
	else
		$stl = null;
	$tc->close();
	return;
}

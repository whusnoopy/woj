<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}
	include('../common/tcpclient.php');
	$rp = array();
	getReport($_GET['jid'], $rp);
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
	<b>Task Report</b><br>
	<hr><br>
	<table border=1>
	<th>User ID</th>
	<th>Name</th>
	<th>ID Card</th>
	<th>Avaliable</th>
	<th>Grade</th>
	<th>Score</th>
<?php
	$cnt = count($rp);
	for ($i=0; $i<$cnt; $i+=6)
		echo '<tr> <td>'.$rp[$i].'</td> <td>'.$rp[$i+1].'</td> <td>'.$rp[$i+2].'</td> <td>'.$rp[$i+3].'</td> <td>'.$rp[$i+4].'</td> <td>'.$rp[$i+5].'</td> </tr>';
?>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;
	<a href="index.php">Home</a>
	</div>
</body>
</html>

<?php
function getReport($jid, &$rp)
{
	$header = sprintf("%s%08d", 'lj', strlen($jid));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($jid) or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$rp = explode("\001", $recv);
	}
	else
		$rp = null;
	$tc->close();
	return;
}

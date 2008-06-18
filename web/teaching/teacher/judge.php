<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'teacher'){
		header('Location: login.php');
		exit;
	}
	$error = '';
	if (!isset($_GET['sid']))
		$error = 'Status ID should not be null!';
	else if (!isset($_GET['pass']))
		$error = 'You should give your judge!';

	if ($error){
		echo $errot.'<br>';
		echo '<a href="javascript:history.back()">Back</a>';
		exit;
	}
	include('../common/tcpclient.php');
	judge($_GET['sid'], $_GET['pass']);
	$jid = $_GET['jid'];
	header("Location: tstatus.php?jid=$jid");
?>

<?php
function judge($sid, $res)
{
	$d = "\001";
	if ($res == 'yes')
		$message = $sid.$d.'1';
	else if ($res == 'no')
		$message = $sid.$d.'2';
	$header = sprintf("%s%08d", 'sr', strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message) or die("send header failed");
	$recv = $tc->recvstr(1);
	return;
}

?>



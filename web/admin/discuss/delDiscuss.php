<?php
	include('../common/tcpclient.php');
	if (isset($_GET['discuss_id']))
		$discuss_id = $_GET['discuss_id'];
	else{
		echo 'no such discuss!';
		exit;
	}
	if (delete_discuss($discuss_id))
		echo 'successfully to delete discuss:'.$discuss_id;
	else
		echo 'failed to delete discuss:'.$discuss_id;

	echo '<br><br><a href=discussList.php>Back</a>';
?>

<?php
function delete_discuss($discuss_id)
{
	$header = sprintf("%s%08d", "dd", strlen($discuss_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($discuss_id)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}
?>
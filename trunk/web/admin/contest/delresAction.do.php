<?php
	include('../common/tcpclient.php');
	if (isset($_GET['contest_id']) && isset($_GET['type'])
		&& set_contest_available($_GET['contest_id'], $_GET['type']))
			header('Location: contestList.php');
	else
		echo "operation failed on contest:[$_GET[contest_id]]";


function set_contest_available($contest_id, $type)
{

	$message = $contest_id."\001".$type;
	$header = sprintf("%s%08d", "bc", strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");

	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();

	return false;
}

?>
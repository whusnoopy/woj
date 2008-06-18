<?php
	include('../common/tcpclient.php');
	if (isset($_GET['problem_id']) && isset($_GET['type'])
		&& set_problem_available($_GET['problem_id'], $_GET['type']))
			header('Location: problemList.php');
	else
		echo "operation failed on problem:[$_GET[problem_id]]";


function set_problem_available($problem_id, $type)
{

	$message = $problem_id."\001".$type;
	$header = sprintf("%s%08d", "bp", strlen($message));

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
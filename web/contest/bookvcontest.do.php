<?php
	include('../common/tcpclient.php');
	$errorMsg = '';

	if (empty($_POST['title']))
		$errorMsg = 'title cannot be NULL';
	else if (empty($_POST['type']))
		$errorMsg = 'type cannot be NULL';
	else if (empty($_POST['start']))
		$errorMsg = 'start time cannot be NULL';
	else if (empty($_POST['end']))
		$errorMsg = 'end time cannot be NULL';
	else if (empty($_POST['p0']))
		$errorMsg = 'problem cannot be NULL';

	if (!empty($errorMsg)){
		echo $errorMsg;
		exit;
	}

	$contest = $_POST['title']
			."\001".$_POST['description']
			."\001".$_POST['start']
			."\001".$_POST['end']
			."\001".$_POST['type'];
	$contest_id = add_contest($contest);
	if (empty($contest_id)){
		echo 'add contest failed!';
		exit;
	}
	echo "add contest successfully! contest_id: [$contest_id]";
	for ($i=0; $i<26; $i++){
		if (empty($_POST['p'.$i])) break;
		$contest_id .= "\001".$_POST['p'.$i];
	}
	if (add_problems_to_contest($contest_id))
		echo 'successfully to add contest !';
	else
		echo 'failed to add contest !';

	echo '<br><br><a href="vcontestList.php">Back</a>';
?>

<?php
function add_contest($contest)
{
	$header = sprintf("%s%08d", "ac", strlen($contest));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
		header('HTTP/1.1 404 Not Found');
		exit;
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($contest)or die("send message failed");
	$recv = $tc->recvstr(10);
	$tc->close();
	sscanf($recv, "%d", $contest_id);
	return $contest_id;
}

function add_problems_to_contest($problemlist)
{
	$header = sprintf("%s%08d", "pc", strlen($problemlist));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
		header('HTTP/1.1 404 Not Found');
		exit;
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problemlist)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}
?>
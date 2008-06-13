<?php
	include('../common/tcpclient.php');
	$errorMsg = '';
	if (empty($_POST['contest_id']))
		$errorMsg = 'contest id error';
	else if (empty($_POST['title']))
		$errorMsg = 'title cannot be NULL';
	else if (empty($_POST['type']))
		$errorMsg = 'type cannot be NULL';
	else if (empty($_POST['start']))
		$errorMsg = 'start time cannot be NULL';
	else if (empty($_POST['end']))
		$errorMsg = 'end time cannot be NULL';

	if (!empty($errorMsg)){
		echo $errorMsg;
		exit;
	}

	$contest = $_POST['contest_id']
			."\001".$_POST['title']
			."\001".$_POST['description']
			."\001".$_POST['start']
			."\001".$_POST['end']
			."\001".$_POST['type'];
	if (update_contest($contest))
		echo 'update contest successfully';
	else
		echo 'update contest failed';

	$contest_id = $_POST['contest_id'];
	for ($i=0; $i<26; $i++){
		if (empty($_POST['p'.$i])) break;
		$contest_id .= "\001".$_POST['p'.$i];
	}

	if (add_problems_to_contest($contest_id))
		echo 'successfully to add problem to contest !';
	else
		echo 'failed to add problem to contest !';

	echo '<br><br><a href="contestList.php">Back</a>';

?>

<?php
function update_contest($contest)
{
	$header = sprintf("%s%08d", "uc", strlen($contest));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($contest)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

function add_problems_to_contest($problemlist)
{
	$header = sprintf("%s%08d", "pc", strlen($problemlist));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
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
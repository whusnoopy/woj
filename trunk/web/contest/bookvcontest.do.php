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
	else if ($_POST['start'] >= $_POST['end'])
		$errorMsg = 'start/end time error';
	else if (!check_time_format($_POST['start']))
		$errorMsg = 'start time format error';
	else if (!check_time_format($_POST['end']))
		$errorMsg = 'end time format error';

	if (!empty($errorMsg)){
		echo $errorMsg.'<br>';
		echo '<a href="javascript:history.back()">Back</a>';
		exit;
	}

	$contest = $_POST['title']
			."\001".$_POST['description']
			."\001".$_POST['start']
			."\001".$_POST['end']
			."\001".$_POST['type'];
	$contest_id = add_contest($contest);
	if (empty($contest_id)){
		echo 'add contest failed!<br>';
		echo '<a href="javascript:history.back()">Back</a>';
		exit;
	}
	for ($i=0; $i<26; $i++){
		if (empty($_POST['p'.$i])) break;
		$contest_id .= "\001".$_POST['p'.$i];
	}
	if (add_problems_to_contest($contest_id))
		echo 'successfully to add contest !';
	else
		echo 'failed to add contest !';

	echo '<br><br><a href="javascript:history.back()">Back</a>';
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

function check_time_format($str_time)
{
	//2008-04-16 12:23:30
	if (strlen($str_time) != 19)
		return false;
	if ($str_time{4} != '-' || $str_time{7} != '-' || $str_time{10} != ' '
		|| $str_time{13} != ':' || $str_time{16} != ':')
		return false;
	for ($i=0; $i<19; $i++)
		if ($i==4 || $i==7){
			if ($str_time{$i} != '-')
			    return false;
		}
		else if ($i==13 || $i==16){
			if ($str_time{$i} != ':')
				return false;
		}
		else if ($i==10){
			if ($str_time{$i} != ' ')
				return false;
		}
		else if ($str_time{$i} < '0' || $str_time{$i} > '9')
			return false;

	$time = strtotime($str_time);
	$now = time();
	if (abs($time - $now) > 365*24*60*60)
		return false;
	return true;

}


?>
<?php
	session_start();

	include('../common/tcpclient.php');

	if (isset($_SESSION['user_id']))
	  $user_id = $_SESSION['user_id'];
	else{
		header("Location: ../user/login.php?errorMsg=Please log in first.");
		exit;
	}

	if (isset($_POST['problem_id']))
		$problem_id = $_POST['problem_id'];
	else
		$problem_id = '0';
	if (isset($_POST['contest_id']))
		$contest_id = $_POST['contest_id'];
	else
		$contest_id = '0';

	if ($_POST['type'] == 'reply'){
		if (add_discuss($_POST['reply_id'], $_POST['topic_id'], $user_id,
			            $problem_id, $contest_id, $_POST['title'],
						$_POST['content'])){
			header("Location: success.php");
			exit;
		}
		else{
			header("Location: error.php?errorMsg=post message error");
			exit;
		}
	}
	else if ($_POST['type'] == 'send'){
		if (add_discuss('0', '-1', $user_id,
			            $problem_id, $contest_id, $_POST['title'],
						$_POST['content'])){
			header("Location: success.php");
			exit;
		}
		else{
			header("Location: error.php?errorMsg=post message error");
			exit;
		}
	}
?>




<?php

function add_discuss($reply_id, $topic_id, $user_id, $problem_id, $contest_id, $title, $content)
{
	/***********/////////////////////////
//	return true;
	/**********//////////////////////////

	$d="\001";
	$message =$reply_id.$d.$topic_id.$d.$user_id.$d.$problem_id.$d.$contest_id.$d.$title.$d.$content;
	$header = sprintf("%s%08d", "ad", strlen($message));

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

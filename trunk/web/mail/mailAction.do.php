<?php
	session_start();
	include('../common/tcpclient.php');
	$to_user = $_POST['to_user'];
	$title = $_POST['title'];
	$content = $_POST['content'];

	if(isset($_POST['type']))
		$type = $_POST['type'];
	else
		$type = $_GET['type'];
	if(isset($_POST['mail_id']))
		$mail_id = $_POST['mail_id'];
	else
		$mail_id = $_GET['mail_id'];

	echo $to_user.'<br>';
	echo $title.'<br>';
	echo $type.'<br>';
	echo $content.'<br>';


	if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else{
		header("Location: mailerror.php?errorMsg=Please log in first.");
		exit;
	}

	if ($type=='delete'){
		if(delete_mail($mail_id, $user_id)){
			header("Location: mailList.php");
			exit;
		}
		else{
			header("Location: mailerror.php?errorMsg=Delete this mail failed!");
			exit;
		}

	}

	if($to_user==null || strlen($to_user) ==0){
		header("Location: mailerror.php?errorMsg=Receiver can't be null.");
		exit;
	}
	if($title==null || strlen($to_user)==0){
		header("Location: mailerror.php?errorMsg=Title can't be null.");
		exit;
	}

	if(!user_exist($to_user)){
		header("Location: mailerror.php?errorMsg=Receiver's id doesn't exist!");
		exit;
	}
	echo 'exist this user<br>';

	if ($type=='send'){

		echo 'now send<br>';
		if (add_mail($to_user, $user_id, $title, $content, 0 )){
			header("Location: mailSuccess.php");
			exit;
		}
		else{
			header("Location: mailerror.php?errorMsg=Send mail failed!");
			exit;
		}
	}
	else if($type=='reply'){
		if (add_mail($to_user, $user_id, $title, $content, 1 )){
			header("Location: mailSuccess.php");
			exit;
		}
		else{
			header("Location: mailerror.php?errorMsg=Send mail failed!");
			exit;
		}
	}

?>

 <?php
function user_exist($user_id)
{
	//////////////////////////////////
//	return true;
	////////////////////////////////
	echo $user_id;
	$header = sprintf("%s%08d", "ei", strlen($user_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($user_id)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

function add_mail($to_user, $user_id, $title, $content, $reply )
{
	/***********/////////////////////////
//	return true;
	/**********//////////////////////////

	$d="\001";
	$message =$reply.$d.$title.$d.$content.$d.$to_user.$d.$user_id;
	$header = sprintf("%s%08d", "am", strlen($message));

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

function delete_mail($mail_id, $user_id)
{
	/***********/
//	return true;
	/**********/
	$message = $mail_id."\001".$user_id;
	$header = sprintf("%s%08d", "dm", strlen($message));

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









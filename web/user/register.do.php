<?php
	include('../common/tcpclient.php');

	if (isset($_POST['submit'])){
		$user_id = $_POST['user_id'];
		$password = $_POST['password'];
		$repeatpassword = $_POST['repeatpassword'];
		$nick = $_POST['nick'];
		$share_code = $_POST['share_code'];
		if($share_code) $share_code = 'Y'; else $share_code = 'N';
		$school = $_POST['school'];
		$email = $_POST['email'];
		$share_email = $_POST['share_email'];
		if($share_email) $share_email = 'Y'; else $share_email = 'N';
		$language = '0';
	}
	else
		exit;

	$errorMsg = '';
	if (empty($user_id))
		$errorMsg = 'User ID can not be NULL';
	else if(strlen($user_id) > 20)
		$errorMsg = 'User ID too long';
	else{
		for($i=0; $i<strlen($user_id); $i++)
			if($user_id{$i} != '_' && !($user_id{$i} >= '0' && $user_id{$i} <= '9')
			   && !($user_id{$i}>= 'A' && $user_id{$i} <= 'Z')
			   && !($user_id{$i} >= 'a' && $user_id{$i} <='z')){
				break;
			}
		if($i != strlen($user_id))
			$errorMsg = 'User ID can only contain number,letter and "_"';
		else if (empty($password))
			$errorMsg = 'Password can not be NULL';
		else if(strlen($password) <6)
			$errorMsg = 'Password must be at least 6 chars';
		else if ($password != $repeatpassword)
			$errorMsg = 'Password do not match';
		else if(existID($user_id))
			$errorMsg = "The ID(".$user_id.")existed";
	}

	if (!empty($errorMsg)){
		header("Location: regerror.php?errorMsg=$errorMsg");
		exit;
	}
	if (register($user_id, $password, $nick, $share_code, $school, $email, $share_email, $language)){
		header("Location: regsuccess.php?user_id=$user_id&nick=$nick&school=$school&email=$email");
		exit;
	}
	else{

		header("Location: regerror.php?errorMsg=sorry, system busy");
		exit;
	}
?>



<?php


	//判断用户是否存在
	function existID($user_id)
	{
		$header = sprintf("%s%08d", "ei", strlen($user_id));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		if (!$tc->connect()){// or die("unable to connect to server!");
			header('HTTP/1.1 404 Not Found');
			exit;
		}
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($user_id) or die("send header failed");
		if($tc->recvstr(1) == 'Y'){
			$tc->close();
			return true;
		}
		$tc->close();
		return false;
	}

	//用户注册
	function register($user_id, $password, $nick, $share_code, $school, $email, $share_email, $language)
	{

		$d = "\001";
		$message = $user_id.$d.$password.$d.$email.$d.$share_email.$d.$nick.$d.$school.$d.$share_code.$d.$language;
		$header = sprintf("%s%08d", "rg", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		if (!$tc->connect()){// or die("unable to connect to server!");
			header('HTTP/1.1 404 Not Found');
			exit;
		}
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message)or die("send message failed");
		if ($tc->recvstr(1) != 's'){
			$tc->close();
			return false;
		}
		$tc->close();
		return true;
	}

?>







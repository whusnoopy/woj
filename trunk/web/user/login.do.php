<?php
	session_start();
	include('../common/tcpclient.php');
	include('../common/get_ip.php');
	if (!empty($_GET['origURL']))
		$origURL = $_GET['origURL'];
	else
	    $origURL = '../index.php';
	if (isset($_POST['doCancel'])){
		header("Location: ../index.php");
		exit;
	}
	if (isset($_POST['doLogin'])){
		if(!isset($_POST['user_id']) || !isset($_POST['password'])){
			header("Location: login.php?errorMsg=The User ID or password do not exist");
			exit;
		}


		$user_id = $_POST['user_id'];
		$password = $_POST['password'];


		if(isset($_POST['autoLogin'])){
			setcookie('user_id', $user_id);
			setcookie('password', $password);
		}
		else{
			setcookie('user_id', '');
			setcookie('password', '');
		}

		if( ($mail_number = login($user_id, $password)) != -1){
			$_SESSION['user_id'] = $user_id;
			$_SESSION['mail_number'] = $mail_number;
			header("Location: $origURL");
			exit;
		}
		else{
			header("Location: login.php?errorMsg=The User ID or password do not exist");
			exit;
		}
	}
?>

<?php
	function login($user_id, $password)
	{
//		if($user_id == 'magiii' && $password == 'mcje2004')
//			return true;
//		return false;

		$ip = get_ip();
		$d = "\001";
		$message=$user_id.$d.$password.$d.$ip;
		$header = sprintf("%s%08d", "li", strlen($message));


		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		if (!$tc->connect()){// or die("unable to connect to server!");
			header('HTTP/1.1 404 Not Found');
			exit;
		}
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message) or die("send header failed");

		if($tc->recvstr(1) == 'Y'){
			$recv = $tc->recvstr(10);
			sscanf("%d", $recv, $mail_number);
			$tc->close();
			return $mail_number;
		}
		$tc->close();
		return -1;
	}
?>

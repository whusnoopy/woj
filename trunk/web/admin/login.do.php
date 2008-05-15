<?php
	session_start();
	include('common/tcpclient.php');
	include('common/get_ip.php');
	if (!empty($_POST['origURL']))
		$origURL = $_POST['origURL'];
	else
		$origURL = 'index.php';

	if(!isset($_POST['user_id']) || !isset($_POST['password'])){
		header("Location: login.php?errorMsg=The User ID or password do not exist");
		exit;
	}

	$user_id = $_POST['user_id'];
	$password = $_POST['password'];

	if(login($user_id, $password)){
		$_SESSION['user_id'] = $user_id;
		$_SESSION['access'] = 'root';
		header("Location: $origURL");
		exit;
	}
	else{
		header("Location: login.php?errorMsg=The User ID or password do not exist");
		exit;
	}

?>
<?php
	function login($user_id, $password)
	{
//		if($user_id == 'magiii' && $password == 'mcje2004')
//			return true;
//		return false;

		$ip=get_ip();
		$d = "\001";
		$message=$user_id.$d.$password.$d.$ip;
		$header = sprintf("%s%08d", "li", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		$tc->connect() or die("unable to connect to server!");
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message) or die("send header failed");
		if($tc->recvstr(1) == 'Y'){
			$tc->close();
			return true;
		}
		$tc->close();
		return false;
	}
?>

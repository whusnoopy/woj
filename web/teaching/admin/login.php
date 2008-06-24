<?php
	session_start();

	if (empty($_GET['error']) && isset($_POST['submit']))
	{
		include('../common/tcpclient.php');
		if (login($_POST['user_id'], $_POST['password'])){
			$_SESSION['user_id'] = $_POST['user_id'];
			//$_SESSION['password'] = $_POST['password'];
			$_SESSION['access'] = 'root';

			echo '<meta http-equiv=refresh content="0;url=index.php">';
			exit;
		}
		else{
			echo '<meta http-equiv=refresh content="0;url=login.php?error=The User ID or password do not exist">';
			exit;
		}
	}

?>

<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Programming Management</title>
  <link href="../style/t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br>
	<b>Admin</b><br>
	<hr><br>
	<?php
		echo $_GET['error'].'<br>';
	?>
	<form action="login.php" method=post onsubmit="encode(this.password.value)">
		User id:<input name="user_id"  maxlength=20 /><br>
		Password:<input type="password" id="ps" name="password" maxlength=20 /><br>
		<input type="submit" name="submit" value="Submit" />
		<input type="reset" value="Reset" />
	</form>
	</div>
<script src="../common/md5.js"></script>
 <script language="javascript">
 function encode(ps)
 {
	 if (ps.length < 32)
		 document.getElementById('ps').value = MD5(ps);
	 return true;
 }
 </script>

</body>
</html>


<?php
function login($user_id, $password)
{
//	if($user_id == 'magiii' && $password == 'mcje2004')
//		return true;
//	return false;

	$ip = get_ip();
	$d = "\001";
	$message=$user_id.$d.$password.$d.$ip;
	$header = sprintf("%s%08d", "li", strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message) or die("send header failed");
	if($tc->recvstr(1) != 'Y'){
		$tc->close();
		return false;
	}
	$tc->close();

	$message = 'A'.$d.$user_id;
	$header = sprintf("%s%08d", 'rt', strlen($message));
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


function get_ip()
{
	if(getenv('HTTP_CLIENT_IP')){
		 $ip = getenv('HTTP_CLIENT_IP');
	}else if(getenv('HTTP_X_FORWARDED_FOR')){
		$ip = getenv('HTTP_X_FORWARDED_FOR');
	}else if ($_SERVER['REMOTE_ADDR']){
		$ip = $_SERVER['REMOTE_ADDR'];
	}
	else
		$ip = 'unknow';
	return $ip;
}
?>


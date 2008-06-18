<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'student'){
		header('Location: login.php');
		exit;
	}
	if (isset($_POST['submit'])){

		include('../common/tcpclient.php');
		include('../common/teaching.config.php');
		if (empty($_POST['pid'])){
			echo 'Hey, you forget to type the Problem ID! <br><a href="javascript:history.back()">Back</a>';
			exit;
		}
		if ($_FILES['file'] <= 0){
			echo 'Hey, you forget to choose a file! <br><a href="javascript:history.back()">Back</a>';
			exit;
		}

		if (!in_array($_FILES['file']['type'], $MIMETYPE)){
			echo 'Sorry, wrong file type!<br><a href="javascript:history.back()">Back</a>';
			exit;
		}

		$d = "\001";
		$sid = submitDoc($_POST['uid']
						.$d.$_POST['pass']
						.$d.$_POST['pid']
						.$d.$_POST['language']
						.$d.get_ip()
						.$d.'F');
		if ($sid <= 0){
			echo 'Sorry, submit failed, please check it! <br><a href="javascript:history.back()">Back</a>';
			exit;
		}
		echo $sid.'<br>';
		$dir = $FILEDIR;
		if (!file_exists($dir))
			mkdir($dir, 0555);


		if (!move_uploaded_file($_FILES['file']['tmp_name'], $dir.$sid.strrchr($_FILES['file']['name'], '.'))) {
			echo 'Sorry, submit failed, please check it! or try it later!<br><a href="javascript:history.back()">Back</a>';
			exit;
		}

		echo 'Submit it successfully!<br><a href="javascript:history.back()">Back</a>';
		exit;
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
	<b>Programming Management</b><br><br>
	<b>Student</b><br>
	<hr><br>
	<b>Submit</b><br><br>
	<form action="submit.php" method="post" enctype="multipart/form-data">
		User ID:<input name="uid" value="<?php echo $_SESSION['user_id']; ?>"><br>
		Password:<input type="password" name="pass"><br>
		Problem ID: <input name="pid"><br>
		Language:<select size=1 name="language">
                 <option value=0>GCC</option> <option value=1 selected>G++</option> <option value=2>Java</option> <option value=3>Pascal</option>
                 </select><br>
		<b>File:</b><br><input type="file" name="file" size="40"><br><br>
		<input type="submit" name="submit" value="Submit"><input type="reset" value="Reset">
	</form>
	</div>
</body>
</html>

<?php
function submitDoc($message)
{
	$d = "\001";
	$header = sprintf("%s%08d", 'sd', strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	return $len;
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
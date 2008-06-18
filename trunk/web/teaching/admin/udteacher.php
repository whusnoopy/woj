<?php
	session_start();
	if (!isset($_SESSION['user_id']) && $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}

	include('../common/tcpclient.php');

	if (isset($_POST['submit'])){
		if (udTeacher($_POST['user_id'], $_POST['realname'])){
			echo 'update teacher:['.$_POST['user_id'].'] successfully!<br>';
		}
		else{
			echo 'update teacher:['.$_POST['user_id'].'] failed!<br>';
		}

		echo '<a href="javascript:history.back()">Back</a>&nbsp;';
		echo '<a href="index.php">Home</a>&nbsp;';
		echo '<a href="teachers.php">Teacher List</a>&nbsp;';
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
	<b>Programming Management</b><br>
	<b>Update teacher's information</b><br>
	<hr><br>
	<form name="form" action="udteacher.php" method=post>
		User ID: <?php echo $_GET['uid']?><br>
		<input type="hidden" name="user_id" value="<?php echo $_GET['uid']?>"/>
		Real Name: <input name="realname"  value="<?php echo $_GET['name']?>"/><br>
		<input type="submit" name="submit" value="Submit">
		<input type="reset" value="Reset">
	</form>
	</div>
</body>
</html>

<?php
function udTeacher($user_id, $realname)
{
	return 0;

	if (empty($user_id) || empty($realname))
		return false;

	$d = "\001";
	$message = $user_id.$d.$realname;
	$header = sprintf("%s%08d", 'ut', $message);

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc-close();
		return true;
	}
	$tc->close();
	return false;
}
?>
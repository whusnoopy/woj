<?php
	session_start();
	if (!isset($_SESSION['user_id']) && $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}

	if (isset($_POST['submit'])){
		include('../common/tcpclient.php');
		if (!ctype_alnum($_POST['user_id']) || empty($_POST['realname']))
			echo 'User ID consist of alphabets and numbers, Realname should not be null!<br>';
		else if (addTeacher($_POST['user_id'], $_POST['realname']))
			echo 'add teacher:['.$_POST['user_id'].'] successfully!<br>';
		else
			echo 'add teacher:['.$_POST['user_id'].'] failed!<br>';

		echo '<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;';
		echo '<a href="index.php">Home</a>';
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
	<b>Add a teacher</b><br>
	<hr><br>
	<form name="form" action="addteacher.php" method=post>
		User ID: <input name='user_id'/><br>
		Real Name: <input name="realname" /><br>
		<input type="submit" name="submit" value="Submit">
		<input type="reset" value="Reset">
	</form>
	</div>
</body>
</html>

<?php
function addTeacher($user_id, $realname)
{
	$d = "\001";
	$message = $user_id.$d.$realname;

	$header = sprintf("%s%08d", 'at', strlen($message));

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
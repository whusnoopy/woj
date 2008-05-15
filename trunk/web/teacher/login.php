<?php
	session_start();

	if (empty($_GET['error']) && isset($_POST['submit']))
	{

		if (login($_POST['user_id'], $_POST['password'])){
			$_SESSION['user_id'] = $_POST['user_id'];
			//$_SESSION['password'] = $_POST['password'];

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
  <link href="t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br>
	<b>Teacher</b><br>
	<hr><br>
	<?php
		echo $_GET['error'].'<br>';
	?>
	<form action="login.php" method=post>
		User id:<input name="user_id"  maxlength=20 /><br>
		Password:<input type="password" name="password" maxlength=20 /><br>
		<input type="submit" name="submit" value="Submit" />
		<input type="reset" value="Reset" />
	</form>
	</div>
</body>
</html>

<?php
	function login($user_id, $password)
	{
		if ($user_id == 'magiii' && $password == 'mcje2004')
			return true;
		return false;
	}
?>
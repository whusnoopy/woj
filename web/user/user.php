<?php
	session_start();
	if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else
		$user_id = '';
?>

<?php
    include('../include/header.php');
	echo '<title>User</title>';
	echo '<div id="tt">User</div>';
	include('../include/notice.php');
?>

<div id=main>

<br>
<?php
	if($user_id==''){
		echo "<span class=bt><a href=login.php>Login</a></span>&nbsp;&nbsp;";
		echo "<span class=bt><a href=register.php>Register</a></span>&nbsp;&nbsp;";
	}
	else{
		echo "<span class=bt><a href=logout.do.php>Logout</a></span>&nbsp;&nbsp;";
		echo "<span class=bt><a href=modify.php>Setting</a></span>&nbsp;&nbsp;";
		echo "<span class=bt><a href=\"../status/userStatus.php?user_id=$user_id\">$user_id</a></span>&nbsp;&nbsp;";
	}
?>
<br><br>
</div>
<?php
	include('../include/tailer.php');
?>
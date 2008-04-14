<?php
	session_start();
	include('../include/header.php');
?>

  <div id="tt">
    User
  </div>

<?php
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
		echo "<span class=bt><a href=../status/userStatus.php>$user_id</a></span>&nbsp;&nbsp;";
	}
?>
<br><br>
</div>
<?php
	include('../include/tailer.php');
?>
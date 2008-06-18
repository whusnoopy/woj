<?php
	session_start();
	unset($_SESSION['user_id']);
	unset($_SESSION['access']);
	$_SESSION = array();
	session_destroy();
	header('Location: login.php');
?>
<?php
session_start();

echo 'hello'.$_SESSION['user_id'];

if (isset($_SESSION['count']))
	$_SESSION['count']++;
else
	$_SESSION['count']=1;

	echo "你一共访问了本页".$_SESSION['count']."次";
?>
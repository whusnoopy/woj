<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location: login.php');
		exit;
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
	1.<a href="arrange.php">Arrange Task</a><br>
	2.<a href="classes.php">Class List</a><br>
	3.<a href="tasks.php">Task List</a><br>
	4.<a href="logout.php">Logout</a><br>
	</div>
</body>
</html>
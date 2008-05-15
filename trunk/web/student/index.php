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
    <link href="s.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br>
	<b>Student</b><br>
	<hr><br>
	1.<a href="task.php">task</a><br>
	2.<a href="logout.php">Logout</a><br>
	</div>
</body>
</html>
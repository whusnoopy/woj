<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'student'){
		header('Location: login.php');
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
	<b>Student</b><br>
	<hr><br>
	1.<a href="tasks.php">Task List</a><br>
	2.<a href="student.php">Information</a><br>
	3.<a href="submit.php">Submit File</a><br>
	4.<a href="logout.php">Logout</a><br>
	</div>
</body>
</html>
<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
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
	<b>Admin</b><br>
	<hr><br>
	1.<a href="teachers.php">Teacher List</a><br>
	2.<a href="addteacher.php">Add Teacher</a><br>
	3.<a href="courses.php">Course List</a><br>
	4.<a href="addcourse.php">Add Course</a><br>
	5.<a href="addstudents.php">Add Series Students</a><br>
	6.<a href="logout.php">Logout</a><br>
	</div>
</body>
</html>
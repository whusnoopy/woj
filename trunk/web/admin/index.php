<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
?>

<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Noah Admin</title>
  <link href="../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="index.php">Home</a>&nbsp;|&nbsp;
    <a href="problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="logout.do.php">Logout</a>
  </div>

  <div id="tt">Welcome to Flood Admin Home</div>

  <div id="main">
    <table width=800><tbody>
    <tr><th>Quick Path</th></tr>
	<tr class=tre><td><a href="setting/setNotice.php">Set notice infomation</a></td></tr>
	<tr class=tro><td><a href="setting/setHomepage.php">Set homepage</a></td></tr>
    <tr class=tre><td><a href="problem/addProblem.php">Add a new problem</a></td></tr>
    <tr class=tro><td><a href="problem/problemList.php">See the problem list</a></td></tr>
    <tr class=tre><td><a href="contest/addContest.php">Add a new contest</a></td></tr>
    <tr class=tro><td><a href="contest/contestList.php">See the contest list</a></td></tr>
    <tr class=tre><td><a href="judge/adminsubmit.php">Submit in Admin's Home</a></td></tr>
    <tr class=tro><td><a href="adminstatus.php">See the status in Admin's Home</a></td></tr>
    <tr class=tre><td><a href="user/addusers.php">Add Series Users</a></td></tr>
    <tr class=tro><td><a href="logout.do.php">Logout</a></td></tr>
    </tbody></table>
  </div>

  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>
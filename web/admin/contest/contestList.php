<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:../login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

<?php
	include('classes/contest_list_t.php');
	if(isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if(isset($_GET['type']))
		$type = $_GET['type'];
	else
		$type = 'N';

	if ($type == 'V')
		echo "<div id=tt>Virtual Concest Volume $start</div>";
	else
		echo "<div id=tt>Concest Volume $start</div>";
?>

<div id="main">
<table ><tbody>
  <tr>
    <th width="50">ID</th>
	<th width="400">Title</th>
	<th width="150">Start</th>
    <th width="150">End</th>
	<th width="60">Status</th>
    <th width="50">Type</th>
	<th width="50">Edit</th>
	<th width="50">Available</th>
  </tr>

<?php

	$cl = new contest_list_t($start, $type);
	$cl->getResult();
	$rows = $cl->getRow();

	for($i=0; $i<$rows; $i++){
		if($i%2==0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		$contestID = $cl->getID($i);
		echo "<td>$contestID</td>";
		echo "<td><a href=contest.php?contest_id=$contestID>".$cl->getTitle($i)."</a></td>";
		echo "<td>".$cl->getStart_time($i)."</td>";
		echo "<td>".$cl->getEnd_time($i)."</td>";
		echo "<td>".$cl->getState($i)."</td>";
		echo "<td>".$cl->get_type($i)."</td>";
		echo "<td><a href=\"editContest.php?contest_id=$contestID&tp=$type\">Edit</td>";
		if ($cl->getAvailable($i) == 'Y')
			echo "<td><a href=\"delresAction.do.php?contest_id=$contestID&type=N\">Delete</a></td>";
		else
			echo "<td><a href=\"delresAction.do.php?contest_id=$contestID&type=Y\">Resume</a></td>";
		echo "</tr>";
	}
?>
</tbody></table>

<br />

<?php
  $current = intval($start);
  if ( $current > 0){
	  $pre = $current - 1;
      echo "<span class=bt><a href=\"contestList.php?start=$pre\">&nbsp;Prev Page&nbsp;</a></span>&nbsp;";
  }
  if ($rows == 20){
	  $next = $current + 1;
      echo "<span class=bt><a href=\"contestList.php?start=$next\">&nbsp;Next Page&nbsp;</a></span>";
  }
?>
  </div><br>


	<div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>
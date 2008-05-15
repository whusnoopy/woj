<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
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
    <a href="problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>


<?php
	include_once('../../common/tcpclient.php');
	include('classes/admin_problem_list_t.php');

	if (isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (empty($user_id))
		$user_id = '?';

	echo "<div id=tt>Problems Volume $start</div>";
?>

  <div id="main">

  <table><tbody align=center>
    <tr>
      <th width="60">&nbsp;</th>
      <th width="100">ID</th>
      <th width="500">Title</th>
	  <th width="120">Ratio&nbsp;(AC/Total)</th>
	  <th width="80">Edit</th>
	  <th width="80">Available</th>
    </tr>
<?php
	$pl = new admin_problem_list_t($start);
	$pl->getResult();
	$rows = $pl->getRow();

	for ($i=0; $i<$rows; $i++){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo '<td></td>';
		$pid = $pl->getProblem_id($i);
		echo "<td>$pid</td>";
		echo "<td><a href=\"problem.php?problem_id=$pid\">".$pl->getTitle($i).'</a></td>';
		$ac = $pl->getAC($i);
		$total = $pl->getTotal($i);
		if (empty($total))
			$ratio = '0.00';
		else
			$ratio = sprintf("%.2f", $ac*100/$total);
		echo "<td>$ratio%($ac/$total)</td>";
		echo "<td><a href=\"editProblem.php?problem_id=$pid\">Edit</a></td>";
		if ($pl->getAvailable($i) == 'Y')
			echo "<td><a href=\"delresAction.do.php?problem_id=$pid&type=N\">Delete</a></td>";
		else
			echo "<td><a href=\"delresAction.do.php?problem_id=$pid&type=Y\">Resume</a></td>";
	}
?>

	</tbody></table>
	<br>


<?php
  $current = intval($start);
  if ( $current > 0){
	  $pre = $current - 1;
      echo "<span class=bt><a href=\"problemList.php?start=$pre\"> &nbsp;Prev Page&nbsp;</a></span>&nbsp;";
  }
  if ($rows == 20){
	  $next = $current + 1;
      echo "<span class=bt><a href=\"problem.php?start=$next\">&nbsp;Next Page&nbsp;</a></span>";
  }
?>
</div>

	<br>

	<div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>


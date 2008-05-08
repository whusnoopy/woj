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
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>


<?php
	include('../common/tcpclient.php');
	include('classes/user_list_t.php');

	if (isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (isset($_GET['user_id']))
		$user_id = $_GET['user_id'];
	else
	    $user_id = '?';

	echo "<div id=tt>User List</div>";
?>

  <div id="main">

  <table><tbody align=center>
    <tr>
      <th width="80">ID</th>
      <th width="350">Nick</th>
	  <th width="160">Last login time</th>
	  <th width="120">Last login IP</th>
	  <th width="160">Register time</th>
	  <th width="80">Details</th>
    </tr>

<?php
	$ul = new user_list_t($start, $user_id);
	$ul->getResult();
	$rows = $ul->getRow();

	for ($i=0; $i<$rows; $i++){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		$uid = $ul->getUser_id($i);
		echo '<td>'.$uid.'</td>';
		echo '<td>'.$ul->getNick($i).'</td>';
		echo '<td>'.$ul->getLast_login_time($i).'</td>';
		echo '<td>'.$ul->getLast_login_ip($i).'</td>';
		echo '<td>'.$ul->getRegister_time($i).'</td>';
		echo "<td><a href=\"userinfo.php?use_id=$uid\">Details</a></td>";

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
  if ($rows == 25){
	  $next = $current + 1;
      echo "<span class=bt><a href=\"problem.php?start=$next\">&nbsp;Next Page&nbsp;</a></span>";
  }
?>


    <form method="get" action="userList.php?start=0" >
	<strong>Search By User ID:</strong>&nbsp;<input name="user_id" type=text  size=40 maxlength="20" />&nbsp;
    <input type="submit" value="GO" />
    </form>
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


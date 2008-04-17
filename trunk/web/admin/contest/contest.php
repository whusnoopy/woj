<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
?>

<?php
	include('../common/tcpclient.php');
	include('classes/contest_problem_list_t.php');
	if (isset($_GET['contest_id']) && !empty($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else{
//		echo "contest doesn't exist";
//		exit;
	}

	$contest = array();
	get_contest_info($contest_id, $contest);

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

	echo "<div id=tt>Contest - ".$contest[0]."</div>";
?>



  <div class="ifm">
<?php
	echo '<strong>Start Time</strong>:'.$contest[2].'&nbsp;&nbsp;';
	echo '<strong>End Time</strong>:'.$contest[3].'<br>';
	$start = strtotime($contest[2]);
	$end = strtotime($contest[3]);
	$current = time()+8*3600;
	echo '<strong>Current Time</strong>:'.date("Y-m-d H:i:s", $current).'&nbsp;&nbsp;';
	echo '<strong>Status</strong>:';
	if ($current > $end)
		echo '<font color=green>Finished</font>';
	else if ($current >= $start)
		echo '<font color=red>Running</font>';
	else
		echo '<font color="green">Started at:&nbsp;'.$contest[2].'</font>';
?>
  </div>


<div id="main">

<table><tbody>
  <tr>
    <th width="50"></th>
    <th width="150">Problem Id</th>
    <th width="550">Title</th>
    <th width="200">AC/Total</th>
  </tr>

<?php
//  if ($current >= $start){
	  $cp = new contest_problem_list_t($contest_id, $_SESSION['user_id']);
	  $cp->getResult();
	  $rows = $cp->getRow();
	  for ($i=0; $i<$rows; $i++){
		  if ($i%2==0)
			  echo '<tr class=tro>';
		  else
			  echo '<tr class=tre>';
		  echo '<td></td>';
		  echo '<td>'.$cp->getProblem_id($i).'</td>';
		  echo '<td><a href=../problem/problem.php?problem_id='.$cp->getProblem_id($i).'>'.$cp->getTitle($i).'</a></td>';
		  $ac = $cp->getAC($i);
		  $total = $cp->getTotal($i);
		  if($total > 0) $ratio = sprintf("%.2f", $ac*100/$total ); else $ratio = '0.00';
		  echo "<td>$ratio%($ac/$total)</td>";
	  }
// }
?>

</tbody></table>
  <br />

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


<?php

function get_contest_info($contest_id, &$contest)
{
	///////////////////////////////
/*	$d = "\001";
	$recv = "2008 Warmup contest 1".$d."hello, flood".$d."2008-03-12 17:00:00".$d."2008-03-15 09:00:00".$d."1";
	$contest = explode("\001", $recv);
	return;
*/	////////////////////////////////

	if(empty($contest_id)){
		$contest = null;
		return;
	}

	$header = sprintf("%s%08d", "cc", strlen($contest_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($contest_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$contest =  explode("\001", $recv);
	}
	else $contest = null;
	$tc->close();
	return;
}
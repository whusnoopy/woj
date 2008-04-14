<?php
	include('../include/header.php');
	include('../common/tcpclient.php');
	include('classes/contest_problem_list_t.php');
	if (empty($user_id))
		$user_id = '?';
	if (isset($_GET['contest_id']) && !empty($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else{
//		echo "contest doesn't exist";
//		exit;
	}

	$contest = array();
	get_contest_info($contest_id, $contest);

?>

<?php

	echo "<div id=tt>Contest - ".$contest[0]."</div>";
	include('../include/notice.php');
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
	if ($current >= $start){
	  $cp = new contest_problem_list_t($contest_id, $user_id);
	  $cp->getResult();
	  $rows = $cp->getRow();
	  if (empty($rows)){
		  echo "It is a private contest";
		  exit;
	  }
	  $ch = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	  for ($i=0; $i<$rows; $i++){
		  if ($i%2==0)
			  echo '<tr class=tro>';
		  else
			  echo '<tr class=tre>';
		  echo '<td></td>';
		  echo '<td>'.$ch{$i}.'</td>';
		  echo '<td><a href=../problem/problem.php?problem_id='.$cp->getProblem_id($i).'>'.$cp->getTitle($i).'</a></td>';
		  $ac = $cp->getAC($i);
		  $total = $cp->getTotal($i);
		  if($total > 0) $ratio = sprintf("%.2f", $ac*100/$total ); else $ratio = '0.00';
		  echo "<td>$ratio%($ac/$total)</td>";
	  }
	}
?>

</tbody></table>
  <br />
<?php
	if ($current >= $start){
		echo '<span class=bt>';
		echo "<a href=\"contestStanding.php?contest_id=$contest_id&start=0&title=$contest[0]\">Standing</a></span>&nbsp;&nbsp";
		echo '<span class=bt>';
		echo "<a href=../status/status.php?contest_id=$contest_id>Status</a></span>&nbsp;&nbsp;";
		echo '<span class=bt>';
		echo "<a href=\"contestStatistics.php?contest_id=$contest_id&title=$contest[0]\">Statistics</a></span>";
	}

?>
  <br /><br />
  </div>


<?php
	include('../include/tailer.php');
?>

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
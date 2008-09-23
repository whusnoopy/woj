<?php
	include('../common/tcpclient.php');
	include('classes/contest_problem_list_t.php');
	if (empty($user_id))
		$user_id = '?';
	if (isset($_GET['contest_id']) && !empty($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else{
		echo "contest doesn't exist";
		exit;
	}

	$contest = array();
	get_contest_info($contest_id, $contest);

?>

<?php
	include('../include/header.php');
	echo '<title>Contest</title>';
	echo "<div id=tt>Contest - ".$contest[0]."</div>";
	include('../include/notice.php');
?>



  <div class="ifm">
<?php
	echo '<strong>Start Time</strong>:'.$contest[2].'&nbsp;&nbsp;';
	echo '<strong>End Time</strong>:'.$contest[3].'<br>';
	$start = strtotime($contest[2]);
	$end = strtotime($contest[3]);
	$current = time();
	echo '<strong>Current Time</strong>:<span id="ctime">';
  echo date("Y-m-d H:i:s", $current).'</span>&nbsp;&nbsp;';
	echo '<strong>Status</strong>:';
	if ($current > $end)
		echo '<font color=green>Finished</font>';
	else if ($current >= $start)
		echo '<font color=red>Running</font>';
	else
		echo '<font color="green">Started at:&nbsp;'.$contest[2].'</font>';
?>
<script language="javascript">
var cstatus = <?php echo $current<$start? 0 : 1; ?>;
var starttime = <?php echo $start; ?> * 1000;
var timestamp = <?php echo time(); ?> * 1000;
function update_time(){
  var y,m,d,g,i,s;
  timestamp+=1000;
  if(timestamp > starttime && cstatus == 0) 
    window.location = window.location;
  var t = new Date(timestamp);
  y=t.getFullYear();
  m=t.getMonth()+1; if(m < 10) m = "0"+m;
  d=t.getDate(); if(d < 10) d = "0"+d;
  g=t.getHours(); if(g < 10) g = "0"+g;
  i=t.getMinutes(); if(i < 10) i = "0"+i;
  s=t.getSeconds(); if(s < 10) s = "0"+s;
  var str = y+"-"+m+"-"+d+" "+g+":"+i+":"+s;
  document.getElementById("ctime").innerHTML = str;
}
setInterval("update_time()", 1000);
</script>
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
		  echo 'It is a private contest<br>';
		  echo '<a href="javascript:history.back()">Back</a>';
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
		  if ($current >= $start && $current <= $end){
			  $problem_html = 'problems/c'.$contest_id.'_'.$ch{$i}.'.html';
			  if (!exist($problem_html))
				  gen_problem_html($problem_html, $contest_id, $cp->getProblem_id($i), $ch{$i});
			  echo '<td><a href="'.$problem_html.'">'.$cp->getTitle($i).'</a></td>';
		  }
		  else
			  echo '<td><a href="../cache/'.$cp->getProblem_id($i).'.html">'.$cp->getTitle($i).'</a></td>';
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

function exist($problem_html)
{
	if (!file_exists($problem_html))
		return false;
	if (time() - filemtime($problem_html) > 60 * 5)
		return false;
	return true;
}

function gen_problem_html($filename, $contest_id, $problem_id, $no)
{
	ob_start();

	$problem = array();
	get_problem_info($problem_id, $problem);
echo '
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>HomePage of Wuhan Univ. Online Judge</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
	<div id="bar">
    <a href="../../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
	<a href="../../contest/vcontestList.php">Virtual Contests</a>&nbsp;|&nbsp;
    <a href="../../submit/submit.php">Submit</a>&nbsp;|&nbsp;
    <a href="../../status/status.php">Status</a>&nbsp;|&nbsp;
    <a href="../../ranklist/ranklist.php">Ranklist</a>&nbsp;|&nbsp;
    <a href="../../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../../user/user.php">User</a>&nbsp;|&nbsp;
    <a href="../../mail/mailList.php">Mail</a>&nbsp;|&nbsp;
    <a href="../../faq.html" target="_blank">FAQ</a>
	</div>
';
echo  '<div id="tt">';
echo "Problem ".$problem_id.' - '.$problem[0];
echo  '</div>
  <div id="move">
  </div>';

echo
	'<div class="ifm">
    <strong>Time Limit</strong>:';
echo $problem[8];
echo 'MS&nbsp;<strong>Memory Limit</strong>:';
echo $problem[9];
echo 'KB<br>';
echo '<strong>Total Submit</strong>:';
echo $problem[10];
echo '&nbsp;<strong>Accepted</strong>:';
echo $problem[11];
echo '<br>
  </div>

  <div id="main">
    <div class="ptt">Description</div>
    <div class="ptx">';
echo $problem[1];
echo '		</div>

    <div class="ptt">Input</div>
    <div class="ptx">';
echo $problem[2];
echo '	</div>

    <div class="ptt">Output</div>
    <div class="ptx">';
echo $problem[3];
echo '	</div>

    <div class="ptt">Sample Input</div>
    <div class="code">';
echo $problem[4];
echo '	</div>

    <div class="ptt">Sample Output</div>
    <div class="code">';
echo $problem[5];
echo '	</div>

    <div class="ptt">Hint</div>
    <div class="ptx">';
echo $problem[6];
echo '	</div>

    <div class="ptt">Source</div>

    <div class="ptx">
		<div>';
echo $problem[7];
echo '		</div>
    </div>
    <br />
    <div>
	  <span class="bt"><a href="../../submit/submit.php?contest_id='.$contest_id.'&problem_id='.$problem_id.'"> Submit</a></span>&nbsp;&nbsp;
	  <span class="bt"><a href="../../discuss/discussList.php?pid='.$problem_id.'">Discuss</a></span>&nbsp;&nbsp;
    </div>
    <br />
  </div>';

	include('../include/tailer.php');
	$buf = ob_get_clean();
	$fp = fopen($filename, 'w') or die("open file $filename failed!");
	fwrite($fp, $buf);
	fclose($fp);
}


function get_problem_info($problem_id, &$problem)
{
	/////////////////////////////
/*	$d="\001";
	$recv = "a + b".$d."we begin".$d."a, b".$d."a+b".$d."1, 2".$d."3".$d."".$d."woj".$d."1000".$d."1024".$d."120".$d."100";
	$problem = explode("\001", $recv);
	return;
*/	//////////////////////////////

	if(empty($problem_id)){
		$problem = null;
		return;
	}

	$header = sprintf("%s%08d", "pb", strlen($problem_id));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
//0		header('HTTP/1.1 404 Not Found');
		exit;
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$problem =  explode("\001", $recv);
	}
	else $problem = null;
	$tc->close();
	return;
}
?>

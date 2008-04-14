<?php
	include('../include/header.php');
    echo '<div id=tt>Status</div>';
	include('../include/notice.php');
	include('../common/tcpclient.php');
	include('classes/status_t.php');
?>


<?php
	if (isset($_SESSION['user_id']) && !empty($_SESSION['user_id']))
		$cur_user_id = $_SESSION['user_id'];
	else
		$cur_user_id = '?';
	if (isset($_GET['start']) && !empty($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (isset($_GET['contest_id']) && !empty($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else
		$contest_id = '?';
	if (isset($_GET['problem_id']) && !empty($_GET['problem_id']))
		$problem_id = $_GET['problem_id'];
	else
	    $problem_id = '0';
	if (isset($_GET['language']) && strlen($_GET['language'])>0 )
	    $language = $_GET['language'];
	else
		$language = '?';
	if (isset($_GET['user_id']) && strlen($_GET['user_id'])>0)
		$user_id = $_GET['user_id'];
	else
		$user_id = '?';
	if (isset($_GET['result']) && !empty($_GET['result']))
		$rst = $_GET['result'];
	else
	    $rst = '?';
	if (isset($_GET['share_code']))
		$share_code = 'Y';
	else
		$share_code = 'N';

    $st = new status_t($start, $problem_id, $user_id, $rst, $language, $contest_id, $share_code, '0' , $cur_user_id);
	$st->getResult();
	$rows = $st->getRow();

?>
  <div id="main">
  <table><tbody>

  <tr>
    <th width="70">Run ID</th>
    <th width="155">User</th>
    <th width="75">Problem</th>
    <th width="200">Result</th>
    <th width="70">Memory</th>
    <th width="60">Time</th>
    <th width="85">Language</th>
    <th width="75">Length</th>
    <th width="160">Submit Time</th>
  </tr>

<?php
	for ($i=0; $i<$rows; $i++){
		if($i%2==0)
		    echo '<tr class=tro>';
		else
		    echo '<tr class=tre>';
		$sid = $st->getSolution_id($i);
		$uid = $st->getUser_id($i);
		$pid = $st->getProblem_id($i);
		$cid = $st->getCode_id($i);
		$rst = $st->getRst($i);
		$mem = $st->getMemory($i);
		$tm = $st->getTime($i);
		$lan = $st->getLanguage($i);
		echo "<td>$sid'</td>";
		echo "<td><a href=\"userStatus.php?user_id=$uid\">$uid</a></td>";
		echo "<td><a href=\"../problem/problem.php?problem_id=$pid}\">$pid</a></td>";
		echo "<td>$rst</td>";
		echo "<td>$mem</td>";
		echo "<td>$tm</td>";
		if ($st->getPermission($i) == 'Y')
			echo "<td><a href=\"../source/source.php?cid=$cid&uid=$uid&pid=$pid&rst=$rst&lan=$lan&tm=$tm&mem=$mem\" tatget=_blank>$lan</a></td>";
		else
			echo "<td>$lan</td>";
		echo '<td>'.$st->getCode_length($i).'</td>';
		echo '<td>'.$st->getIn_date($i).'</td>';
		echo '</tr>';
	}
?>
  </tbody></table>

  <br />
  <form action="status.php" method="get">
   <strong>Select status by</strong>&nbsp;
   <strong>Problem ID:</strong> <input size=6 name="problem_id" />&nbsp;
   <strong>User ID:</strong> <input size=14 name="user_id" />&nbsp;
   <strong>Result:</strong> <select size=1 name="result" />
    <option value="?" selected>All</option>
    <option value=1>Accepted</option>
    <option value=2>Presentation Error</option>
    <option value=3>Time Limit Exceed</option>
    <option value=4>Memory Limit Exceed</option>
    <option value=5>Wrong Answer</option>
    <option value=6>Output Limit Exceed</option>
    <option value=7>Compile Error</option>
    <option value=8>Runtime Error(SIGSEGV)</option>
    <option value=9>Runtime Error(SIGFPE)</option>
    <option value=10>Runtime Error(SIGBUS)</option>
    <option value=11>Runtime Error(SIGABRT)</option>
    <option value=12>Runtime Error(JAVA)</option>
    <option value=13>Runtime Error(Pascal)</option>
    <option value=14>Restricted Function</option>
    <option value=15>System Error</option>
   </select>&nbsp;
   <strong>Language:</strong> <select size=1 name="language" >
    <option value="?" selected>All</option>
    <option value="0">GCC</option>
    <option value="1">G++</option>
    <option value="2">Java</option>
    <option value="3">Pascal</option>
   </select>&nbsp;
   <strong>shared:</strong><input size="30" type="checkbox" name="share_code" value=true/>&nbsp;&nbsp;
   <input type=submit value="Go" width="8" />
  </form>
  <br />
  <div>

<?php
	if($start > 0){
		echo "<span class=bt><a href=\"status.php?start=0&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Top</a></span>&nbsp";
       $pre = $start - 1;
	   echo "<span class=bt><a href=\"status.php?start=$pre&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Top</a></span>&nbsp";
	}
    if ($rows == 20){
       $next = $start + 1;
	   echo "<span class=bt><a href=\"status.php?start=$next&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Top</a></span>&nbsp";
	}
?>
</div>
  <br />
  </div>


<?php
	include('../include/tailer.php');
?>
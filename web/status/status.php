<?php
	session_start();
	include('../common/tcpclient.php');
	include('../common/config.php');
	include('classes/status_t.php');

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

    $st = new status_t($start, $problem_id, $user_id, $rst, $language, $contest_id, $share_code, 'N' , $cur_user_id);
	$st->getResult();
	$rows = $st->getRow();

  if(!isset($_GET['result']))
    echo '<meta http-equiv="Refresh" content="30; url=status.php"/>';

	include('../include/header.php');
?>
	<title>Status</title>
  <div id=tt>Status</div>
<?php
	include('../include/notice.php');
?>
  <div id="main">
  <p id="search">
  <form action="status.php" method="get">
   <strong>Search by</strong>&nbsp;
   <strong>Problem ID:</strong> <input size=6 name="problem_id" />&nbsp;
   <strong>User ID:</strong> <input size=14 name="user_id" />&nbsp;
   <strong>Result:</strong> <select size=1 name="result" />
    <option value="?" selected>All</option>
    <option value=1>ACCEPT</option>
    <option value=2>WRONG_ANSWER</option>
    <option value=3>PRESENTATION_ERROR</option>
    <option value=4>COMPILE_ERROR</option>
    <option value=5>TIME_LIMIT_EXCEEDED</option>
    <option value=6>MEMORY_LIMIT_EXCEEDED</option>
    <option value=7>OUTPUT_LIMIT_EXCEEDED</option>
    <option value=8>RUNTIME_ERROR_SIGSEGV</option>
    <option value=9>RUNTIME_ERROR_SIGFPE</option>
    <option value=10>RUNTIME_ERROR_SIGBUS</option>
    <option value=11>RUNTIME_ERROR_SIGABRT</option>
    <option value=12>RUNTIME_ERROR_JAVA</option>
    <option value=13>RESTRICTED_FUNCTION</option>
    <option value=14>SYSTEM_ERROR</option>
    <option value=15>PENDING</option>
  <option value=16>COMPILING</option>
  <option value=17>RUNNING</option>
  <option value=18>JUDGING</option>
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
  </p>
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
		$result = $st->getRst($i);
		$mem = $st->getMemory($i); $mem = $mem<0 ? '' : $mem;
		$tm = $st->getTime($i);  $tm = $tm<0 ? '' : $tm;
		$lan = $st->getLanguage($i);
		$eid = $st->getError_id($i);
		echo "<td>$sid</td>";
		echo "<td><a href=\"userStatus.php?user_id=$uid\">$uid</a></td>";
		echo "<td><a href=\"../problem/cache/{$pid}.html\">$pid</a></td>";
		if (empty($eid)){
      switch(strtoupper($JUDGE_STATUS[$result])){
        case 'UNKNOW':
            $status = 'Waiting...'; $color = '#0000dd'; break;
        case "ACCEPTED":
        case 'ACCEPT':
            $status = 'Accepted'; $color = '#dd0000'; break;
        case 'WRONG_ANSWER':
            $status = 'Wrong Answer'; $color = '#858530'; break;
        case 'PRESENTATION_ERROR': 
            $status = 'Presentation Error'; $color = '#00dd00'; break;
        case 'MEMORY_LIMIT_EXCEEDED':
            $status = 'Memory Limit Exceeded'; $color = '#660066'; break;
        case 'TIME_LIMIT_EXCEEDED':
            $status = 'Time Limit Exceeded'; $color = '#66bb66'; break;
        case 'RUNTIME_ERROR_SIGSEGV':
            $status = 'Runtime Error (SIGSEGV)'; $color = '#707070'; break;
        case 'RUNTIME_ERROR_SIGFPE': 
            $status = 'Runtime Error (SIGFPE)'; $color = '#707050'; break;
        default:
            $status = $JUDGE_STATUS[$result]; $color = '#000000'; break;
      }
			echo '<td style="color:' . $color . '">'. $status .'</td>';
    }
		else
			echo "<td><a href=\"compileerror.php?ce_id=$eid\" target=\"_balnk\">".$JUDGE_STATUS[$result].'</a></td>';
		echo "<td>$mem</td>";
		echo "<td>$tm</td>";
		if ($st->getPermission($i) == 'Y')
			echo "<td><a href=\"../source/source.php?cid=$cid&uid=$uid&pid=$pid&rst=$result&lan=$lan&tm=$tm&mem=$mem&sid=$sid\" target=_blank>".$LANGUAGE[$lan]."</a></td>";
		else
			echo "<td>".$LANGUAGE[$lan]."</td>";
		echo '<td>'.$st->getCode_length($i).'</td>';
		echo '<td>'.$st->getIn_date($i).'</td>';
		echo '</tr>';
	}
?>
  </tbody></table>

  <br />
  <br/>
  <div>

<?php
	if($start > 0){
		echo "<span class=bt><a href=\"status.php?start=0&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Top Page</a></span>&nbsp";
       $pre = $start - 1;
	   echo "<span class=bt><a href=\"status.php?start=$pre&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Previous Page</a></span>&nbsp";
	}
    if ($rows == 25){
       $next = $start + 1;
	   echo "<span class=bt><a href=\"status.php?start=$next&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Next Page</a></span>&nbsp";
	}
?>
</div>
  <br />
  </div>


<?php
	include('../include/tailer.php');
?>

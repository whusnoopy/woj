<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:../login.php?errorMsg=please login first!');
		exit;
	}
?>
<?php
	include('../common/tcpclient.php');
	include('classes/status_t.php');
	include('../common/config.php');

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

    $st = new status_t($start, $problem_id, $user_id, $rst, $language, $contest_id, $share_code, 'A' , $cur_user_id);
	$st->getResult();
	$rows = $st->getRow();

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
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

<div id=tt>Judge</div>
  <div id="main">
  <table><tbody>

  <div align="center">
  <input type=button name="chooseall" value="Choose all" onclick="allChoose(<?php echo $rows; ?>);"/>
  <input type=button name="cancelall" value="Cancel all" onclick="allCancel(<?php echo $rows; ?>);"/>
  </div>

  <form action="rejudgeAction.do.php" name="form0" method="post" onSubmit="return makesure('Rejudge these solutions now?');" >
  <input type="submit" value="Rejudge">
  <tr>
    <th width="20"></th>
    <th width="70">Run ID</th>
    <th width="155">User</th>
    <th width="75">Problem</th>
    <th width="180">Result</th>
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
		$mem = $st->getMemory($i);
		$tm = $st->getTime($i);
		$lan = $st->getLanguage($i);

		echo "<td><input type=checkbox name=\"s${i}\" value=\"$sid\"></td>";
		echo "<td>$sid</td>";
		echo "<td>$uid</td>";
		echo "<td>$pid</td>";
		echo '<td>'.$JUDGE_STATUS[$result].'</td>';
		echo "<td>$mem</td>";
		echo "<td>$tm</td>";
		echo '<td>'.$LANGUAGE[$lan].'</td>';
		echo '<td>'.$st->getCode_length($i).'</td>';
		echo '<td>'.$st->getIn_date($i).'</td>';
		echo '</tr>';
	}
?>
  </form>

  </tbody></table>

  <br />
  <form action="judge.php" method="get">
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
		echo "<span class=bt><a href=\"judge.php?start=0&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Top</a></span>&nbsp";
       $pre = $start - 1;
	   echo "<span class=bt><a href=\"judge.php?start=$pre&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Previous</a></span>&nbsp";
	}
    if ($rows == 25){
       $next = $start + 1;
	   echo "<span class=bt><a href=\"judge.php?start=$next&contest_id=$contest_id&problem_id=$problem_id&result=$rst&user_id=$user_id&language=$language\">Next</a></span>&nbsp";
	}
?>
</div>
  <br />
  </div>

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

<script language="javascript">
<!--
function makesure(msg)
{
	return confirm(msg);
}

function allChoose(row)
{
	for (i=1; i<=row; i++)
		document.form0.elements[i].checked = true;
}

function allCancel(row)
{
	for (i=1; i<=row; i++)
		document.form0.elements[i].checked = false;
}
//-->
</script>
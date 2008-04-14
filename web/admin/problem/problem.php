<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:login.php?errorMsg=please login first!');
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
    <a href="problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

<?php
	$problem_id = $_GET['problem_id'];
	$problem = array();
	getProblemDetails($problem_id, $problem);
?>

<div id="tt">
    <?php echo 'Problem '.$problem_id.' - '.$problem[0];?>
</div>

<div id="main">
    <div class="ptt">Description</div>
    <div class="ptx"><?php echo $problem[1]; ?></div>

    <div class="ptt">Input</div>
    <div class="ptx"><?php echo $problem[2]; ?></div>

    <div class="ptt">Output</div>
    <div class="ptx"><?php echo $problem[3]; ?></div>

    <div class="ptt">Sample Input</div>
    <div class="code"><?php echo $problem[4]; ?></div>

    <div class="ptt">Sample Output</div>
    <div class="code"><?php echo $problem[5]; ?></div>

    <div class="ptt">Hint</div>
    <div class="ptx"><?php echo $problem[6]; ?></div>

    <div class="ptt">Source</div>
    <div class="ptx"><?php echo $problem[7]; ?></div>

    <div class="ptt">Add in time</div>
    <div class="ptx"><?php echo $problem[8]; ?></div>

	<div class="ptt">Time limit</div>
    <div class="ptx"><?php echo $problem[9]; ?></div>

	<div class="ptt">Case time limit</div>
    <div class="ptx"><?php echo $problem[10]; ?></div>

    <div class="ptt">Memory limit</div>
    <div class="ptx"><?php echo $problem[11]; ?></div>

    <div class="ptt">Standard time limit</div>
    <div class="ptx"><?php echo $problem[12]; ?></div>

    <div class="ptt">Standart memory limit</div>
    <div class="ptx"><?php echo $problem[13]; ?></div>

    <div class="ptt">version</div>
    <div class="ptx"><?php echo $problem[14]; ?></div>

    <div class="ptt">Special judge</div>
    <div class="ptx"><?php echo $problem[15]; ?></div>

</div>
<br />



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
function getProblemDetails($problem_id, &$problem)
{
	$d = "\001";
	$header = sprintf("%s%08d", "mp", strlen($problem_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$problem = explode($d, $recv);
	}
	else
		$problem = null;
	$tc->close();
}
?>
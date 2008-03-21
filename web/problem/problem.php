<?php

	if (isset($_GET['problem_id']))
		$problem_id = intval( $_GET['problem_id'] );
	else
		$problem_id = 0;

	$problem = get_problem_info($problem_id);
	if (count($problem) < 12){
		for($i = count($problem); $i <= 12; $i++)
			$problem[] = ' ';
	}
?>

<?php
	include('../include/header.php');
	include('../common/tcpclient.php');
?>

  <div id="tt">
    <?php echo "Problem ".$problem_id.' - '.$problem[0];?>
  </div>

<?php
	include('../include/notice.php');
?>

  <div class="ifm">
    <strong>Time Limit</strong>:<?php echo $problem[8]; ?>MS&nbsp;<strong>Memory Limit</strong>:<?php echo $problem[9]; ?>KB<br>
    <strong>Total Submit</strong>:<?php echo $problem[10]; ?>&nbsp;<strong>Accepted</strong>:<?php echo $problem[11]; ?><br>
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

    <div class="ptx">
		<div><?php echo $problem[7]; ?></div>
    </div>
    <br />
    <div>
      <span class="bt"><a href="../submit/submit.php?problem_id=<?php echo $problem_id;?>">Submit</a></span>&nbsp;&nbsp;
	  <span class="bt"><a href="../submit/discuss.php?problem_id=<?php echo $problem_id;?>">Discuss</a></span>&nbsp;&nbsp;
      <span class="bt"><a href="../status/problemstatus.php?problem_id=<?php echo $problem_id;?>">Status</a></span>
    </div>
    <br />
  </div>

<?php
	include('../include/tailer.php');
?>

<?php

function get_problem_info($problem_id)
{
	/////////////////////////////
	$d="\001";
	$recv = "a + b".$d."we begin".$d."a, b".$d."a+b".$d."1, 2".$d."3".$d."".$d."woj".$d."1000".$d."1024".$d."120".$d."100";
	return explode("\001", $recv);
	//////////////////////////////

	if(empty($problem_id)) return null;

	$header = sprintf("%s%08d", "pb", strlen($problem));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	$len = sscanf($recv, "%d");
	if($len > 0){
		$recv = $tc->recvstr($len);
		$tc->close();
		return explode("\001", $recv);
	}
	$tc->close();
	return null;
}
?>

<%--  cannot be copy  --%>
<body onmouseup=document.selection.empty() oncontextmenu="return false" onselectstart="return false" ondragstart="return false" onbeforecopy="return false" oncopy=document.selection.empty() leftMargin=0 topMargin=0 onselect=document.selection.empty() >
<center>

<?php
    include('../include/header.php');
	include('classes/format_code_t.php');

	if(isset($_GET['solution_id']))
		$solution_id = $_GET('solution_id');
	else
		$solution_id = '0';

	$solution_info = 1;
	$isValid = 1;
	$problem["title"] = $_GET['tt'];
	$problem["id"] = $_GET['id'];
	$problem["uid"] = $_GET['uid'];
	$problem["limit_time"] = $_GET['tm'];
	$problem["memory"] = $_GET['mem'];
	$problem["language"] = $_GET['lan'];
	$problem["result"] = $_GET['res'];
	$problem["source"] = getSource($solution_id, $user_id);
?>

<?php

 if(empty($solution_info))
   echo  ' <div><br /><span class="cl">No such solution id</span></div><br /> ';

 else if (empty($problem["source"]))
   echo ' <div><br /><span class="cl">You have no access to view this code</span></div><br /> ';
 else{
	 $fc = new format_code_t($problem["source"], "G++");
?>
  <div id="tt">Source - <?php echo $problem["title"];?></div>
  <?php include('..\include\notice.php'); ?>
   <div class="ifm">
   <strong>Problem id</strong>: <?php echo $problem["id"];?>&nbsp;&nbsp;
   <strong>User id</strong>: <?php echo $problem["uid"];?><br>
   <strong>Memory</strong>: <?php echo $problem["memory"];?>KB&nbsp;&nbsp;
   <strong>Time</strong>: <?php echo $problem["limit_time"];?>ms<br>
   <strong>Language</strong>: <?php echo $problem["language"];?>&nbsp;&nbsp;
   <strong>Result</strong>: <?php echo $problem["result"];?><br>
  </div>

  <div id="main">
  <div class="ptt">Code</div>
  <div class="code"><?php echo $fc->getResultSource();?></div>
  </div>

<?php
 }
?>


<?php
	include('../include/tailer.php');
?>

<?php
function getSource($solution_id, $user_id)
{
	return
"#include<stdio.h>
int main()
{
	return 0;  //hello, magiii
}";

	$message = $solution_id."\001".$user_id;
	$header = sprintf("%s%08d", "sc", strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message);
	$recv = $tc->recvstr(1);
	if($recv=="N"){//没有阅读代码的权限
		$tc->close();
		return '';
	}
	$recv = $tc->recvstr(10);
	$len = sscanf($recv, "%d");
	$recv = $tc->recvstr($len);
	$tc->close();
	return $recv;
}

?>


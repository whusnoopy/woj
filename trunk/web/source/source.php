
<%--  cannot be copy  --%>
<body onmouseup=document.selection.empty() oncontextmenu="return false" onselectstart="return false" ondragstart="return false" onbeforecopy="return false" oncopy=document.selection.empty() leftMargin=0 topMargin=0 onselect=document.selection.empty() >
<center>

<?php
    include('..\include\header.php');

	if(isset($_GET('solution_id']))
		$solution_id = $_GET('solution_id');
	else
		$solution_id = '0';

	$solution_info = 1;
	$isValid = 1;
	$problem["title"] = "a+b";
	$problem["id"] = "1000";
	$problem["uid"] = "magiii";
	$problem["limit_time"] = 1000;
	$problem["memory"] = 1024;
	$problem["language"] = "GCC";
	$problem["result"] = "Accept";
	$problem["source"]= "include &lt;stdio.h&gt;</br> int main()</br>{</br>return 0;</br>}</br>";
?>

<?php

 if($solution_info==null)
   echo  ' <div><br /><span class="cl">No such solution id</span></div><br /> ';

 else if ($isValid != 1)
   echo ' <div><br /><span class="cl">You have no access to view this code</span></div><br /> ';
 else{
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
  <div class="code"><?php echo $problem["source"];?></div>
  </div>

<?php
?>


<?php
	include('..\include\tailer.php');
?>

<?php

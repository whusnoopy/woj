<?php
	session_start();
	if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else
		$user_id = '';
	include('../common/tcpclient.php');
	include('../common/config.php');
	include('classes/format_code_t.php');

	if(isset($_GET['cid']))
		$code_id = $_GET['cid'];
	else{
//		echo 'solution does not exist';
//		exit;
	}
	$language_type = array('GCC','G++','JAVA','PASCAL');
	$problem["sid"] = $_GET['sid'];
	$problem["pid"] = $_GET['pid'];
	$problem["uid"] = $_GET['uid'];
	$problem["limit_time"] = $_GET['tm'];
	$problem["memory"] = $_GET['mem'];
	$problem["language"] = $language_type[$_GET['lan']];
	$problem["result"] = $_GET['rst'];
	$problem["source"] = getSource($code_id, $user_id);

?>

<?php
 include('../include/header.php');
 if (empty($problem["source"]))
   echo ' <div><br /><span class="cl">You have no access to view this code</span></div><br /> ';
 else{
	 $fc = new format_code_t($problem["source"], "G++");
?>
 <body onmouseup=document.selection.empty() oncontextmenu="return false" onselectstart="return false" ondragstart="return false" onbeforecopy="return false" oncopy=document.selection.empty() leftMargin=0 topMargin=0 onselect=document.selection.empty() >
  <div id="tt">Source - <?php echo $problem["sid"];?></div>
  <?php include('..\include\notice.php'); ?>
   <div class="ifm">
   <strong>Problem id</strong>: <?php echo $problem["pid"];?>&nbsp;&nbsp;
   <strong>User id</strong>: <?php echo $problem["uid"];?><br>
   <strong>Memory</strong>: <?php echo $problem["memory"];?>KB&nbsp;&nbsp;
   <strong>Time</strong>: <?php echo $problem["limit_time"];?>ms<br>
   <strong>Language</strong>: <?php echo $problem["language"];?>&nbsp;&nbsp;
   <strong>Result</strong>: <?php echo $JUDGE_STATUS[$problem["result"]];?><br>
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
function getSource($code_id, $user_id)
{
	$message = $code_id."\001".$user_id;
	$header = sprintf("%s%08d", "sc", strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
		header('HTTP/1.1 404 Not Found');
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message);
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0)
		$recv = $tc->recvstr($len);
	else
		$recv = null;
	$tc->close();
	return $recv;
}

?>


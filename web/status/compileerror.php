<?php
	header('HTTP/1.1 404 NotFound');
	include('../include/header.php');
	$ce_id = $_GET['ce_id'];
	echo '<div id=tt>Error</div>';
	include('../include/notice.php');
	include('../common/tcpclient.php');
?>

 <div id="main">
   <br />
   <div class="ptt">Information</div>
   <div class="code"><pre><?php echo getErrorInfomation($ce_id); ?></pre></div>
 </div>

<?php
	include('../include/tailer.php');
?>

<?php
function getErrorInfomation($ce_id)
{
	$header = sprintf("%s%08d", 'ec', strlen($ce_id));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($ce_id);
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

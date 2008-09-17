<?php
include('../common/tcpclient.php');
function get_notice()
{
	$header = "np00000000";

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
//	$tc->sendstr($problem_id)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0)
		$recv = $tc->recvstr($len);
	else
		$recv = null;
	$tc->close();
	return $recv;
}
echo trim(get_notice());
?>

<?php
include ('..\common\tcpclient.php');



//for($i=0; $i<1000; $i++){
	$tc = new TCPClient();
	$tc->create();
	$tc->connect();
	$data = '0001111111';
	$tc->sendstr($data);

	$tc->close();
//}
?>

<?php
include ('..\common\tcpclient.php');



//for($i=0; $i<1000; $i++){
	$tc = new tcpclient("127.0.0.1", 1234);
	$tc->create();
	$tc->connect();
	$data = '0001';
	$tc->sendstr($data);

	$tc->close();
//}
?>

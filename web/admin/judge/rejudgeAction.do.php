<?php
	include('../common/tcpclient.php');
	for ($i=0; $i<25; $i++){
		$sid = 's'.$i;
		if (isset($_POST[$sid])){
			if (rejudge($_POST[$sid]))
				echo 'solution: <font color=red>'.$_POST[$sid].'</font> has been rejudged!<br>';
			else
				echo 'failed to rejudge solution: <font color=red>'.$_POST[$sid].'</font><br>';
		}
	}

	echo '<br><a href="judge.php">view result!</a>';


function rejudge($sid)
{

	$header = sprintf("%s%08d", "rj", strlen($sid));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($sid) or die("send message failed");
	if($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}


?>

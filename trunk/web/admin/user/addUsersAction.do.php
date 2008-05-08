<html>
</body>
<?php

	include('../common/tcpclient.php');
	if (isset($_POST['file'])){
		echo 'file';
	}
	else{
		$names = array();
		$names = explode("\r\n", $_POST['username']);
		$cnt = count($names);
		if ($cnt>999){
			echo 'too many users';
			exit;
		}
		$username = array();
		$pass = array();
		$valid = array();
		$d = "\001";
		$message = $cnt;

		for ($i=0; $i<$cnt; $i++){
			$name[$i] = trim($name[$i]);
			$username[] = $_POST['suffix'].$names[$i].($i<100 ? ($i<10 ? '00'.$i : '0'.$i) : $i);
			$pass[] = genPassword();
			$message .= ($d.$username[$i].$d.$pass[$i]);
		}

		add_series_users($message, $valid);

		$fp = fopen('users_file.txt', 'w');
		for ($i=0; $i<$cnt; $i++){
			if ($valid[$i] == 'Y')
				fprintf($fp, "%20s%20s\r\n", $username[$i], $pass[$i]);
		}
		fclose($fp);

		echo '<a href="users_file.txt" target="_blank">users_file.txt</a><br>';
		echo '<pre>';
		printf("%20s%20s%20s\n", 'username', 'password', 'result');
		printf("----------------------------------------------------------------------\n");
		for ($i=0; $i<$cnt; $i++){
			if ($valid[$i] == 'Y')
				printf("<font color=green>%20s%20s%20s</font>\n", $username[$i], $pass[$i], 'successfully');
			else
				printf("<font color=red>%20s%20s%20s</font>\n", $username[$i], $pass[$i], 'failed');
		}
		echo '</pre>';
	}

?>
</body>
</html>

<?php
function genPassword()
{
	$words = '1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$pass= '';
	//no need srand() from php4.2.0
	for ($i=0; $i<8; $i++)
		$pass .= $words{rand(0,61)};
	return $pass;
}

function add_series_users($message, &$valid)
{
	$d = "\001";
	$header = sprintf("%s%08d", "su", strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$valid = explode($d, $recv);
	}
	else
		$valid = null;
	$tc->close();
}
?>

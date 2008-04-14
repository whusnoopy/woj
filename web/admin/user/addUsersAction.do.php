<html>
</body>
<?php

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
		$fp = fopen('users_file.txt', 'w');
		for ($i=0; $i<$cnt; $i++){
			$username[] = $_POST['suffix'].$names[$i].($i<100 ? ($i<10 ? '00'.$i : '0'.$i) : $i);
			$pass[] = genPassword();
			fprintf($fp, "%20s%20s\r\n", $username[$i], $pass[$i]);
		}
		fclose($fp);

		echo '<a href="users_file.txt" target="_blank">users_file.txt</a><br>';
		echo '<pre>';
		printf("%20s%20s\n", 'username', 'password');
		printf("----------------------------------------------------------------------\n");
		for ($i=0; $i<$cnt; $i++)
			printf("%20s%20s\n", $username[$i], $pass[$i]);
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
?>

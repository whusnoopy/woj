<?php

    echo "we are dealing with you request now, please wait...";
	include ('../common/tcpclient.php');

	if (isset($_POST['submit'])){
		$user_id = $_POST['user_id'];
		$oldPass = $_POST['oldPass'];
		$newPass = $_POST['newPass'];
		$rptNewPass = $_POST['rptNewPass'];
		$nick = $_POST['nick'];
		$share_code = $_POST['share_code'];
		if($share_code) $share_code = 'Y'; else $share_code = 'N';
		$school = $_POST['school'];
		$email = $_POST['email'];
		$share_email = $_POST['share_email'];
		if($share_email) $share_email = 'Y'; else $share_email = 'N';
		$language = '0';
	}

	if($newPass != $rptNewPass){
		header("Location: modifyerr.php?errorMsg=The New Password doesn't match");
		exit;
	}
	if(empty($newPass))
		$newPass = $oldPass;

	if(update_user_info($user_id, $oldPass, $newPass, $email,
							$share_email, $nick, $school, $share_code, $language)){
		header("Location: modifysuc.php?user_id=$user_id&nick=$nick&school=$school&email=$email");
		exit;
	}
	else{
		header("Location: modifyerr.php?errorMsg=system is busy");
		exit;
	}

?>

<?php

	function update_user_info($user_id, $oldPass, $newPass, $email,
							$show_email, $nick, $school, $share_code, $language)
	{
//		return true;

		$d="\001";
		$message = $user_id.$d.$oldPass.$d.$newPass.$d.$email.$d.$show_email.$d.$nick.$d.$school.$d.$share_code.$d.$language;
		$header = sprintf("%s%08d", "uu", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		$tc->connect() or die("unable to connect to server!");
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message) or die("send header failed");
		if($tc->recvstr(1) == 'Y'){
			$tc->close();
			return true;
		}
		$tc->close();
		return false;
	}
?>




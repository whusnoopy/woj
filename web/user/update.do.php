<?php
  ob_start();
	include ('../common/tcpclient.php');

	if (isset($_POST['submit'])){
    var_dump($_POST);exit;
    extract($_POST);
    if(empty($nick))$nick = $user_id;
		if($share_code) $share_code = 'Y'; else $share_code = 'N';
    if(empty($school))$school = " ";
    if(empty($email))$email = " ";
		if($share_email) $share_email = 'Y'; else $share_email = 'N';
		$language = '0';
	}else{
    header("Location: modify.php");
    exit;
  }

	if(empty($newPass)){
		$newPass = $oldPass;
  }else{
    if($newPass != $rptNewPass){
      header("Location: modifyerr.php?errorMsg="
          . urlencode("The new password doesn't match the repeated one..."));
      exit;
    }
  }

	if(update_user_info($user_id, $oldPass, $newPass, $email,
							$share_email, $nick, $school, $share_code, $language)){
		header("Location: modifysuc.php?"
        . "user_id=" . urlencode($user_id)
        . "&nick=" . urlencode($nick)
        . "&school=" . urlencode($school)
        . "&email=" .urlencode($email));
		exit;
	} else{
		header("Location: modifyerr.php?errorMsg=system+is+busy");
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




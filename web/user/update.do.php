<?php

    echo "we are dealing with you request now, please wait...";

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

	if(!confirm($user_id, $oldPass)){
		header("Location: modifyerr.php?errorMsg=The ID( $user_id ) is not existed or password is not correct");
		exit;
	}
	if($newPass != $rptNewPass){
		header("Location: modifyerr.php?errorMsg=The New Password doesn't match");
		exit;
	}
	if(empty($newPass))
		$newPass = $oldPass;

	if(update_user_info($user_id, $newPass, $nick, $share_code, $school, $email, $share_email, $language)){
		header("Location: modifysuc.php?user_id=$user_id&nick=$nick&school=$school&email=$email");
		exit;
	}
	else{
		header("Location: modifyerr.php?errorMsg=system is busy");
		exit;
	}

?>

<?php
function confirm($user_id, $oldPass)
{
	return true;
}

function update_user_info($user_id, $newPass, $nick, $share_code, $school, $email, $share_email, $language )
{
	return true;
}
?>




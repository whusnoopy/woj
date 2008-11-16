<?php
	session_start();
  if(isset($_POST['problem_id'])){
    $_SESSION['problem_id'] = $_POST['problem_id'];
  }
	include('../common/tcpclient.php');
	include('../common/get_ip.php');
	$errorMsg = '';
	$source_len = 0;

	if(empty($_POST['user_id']) || empty($_POST['pass'])){
		$errorMsg = 'Invalid user ID or password!';
	}
	else if(empty($_POST['problem_id'])){
		$errorMsg = 'please type your problem_id!';
	}
	else if(empty($_POST['language'])){
		$errorMsg = 'please type your language!';
    }
    else if($_POST['language'] == 2){//java
        $errorMsg = 'Java Compiler doesn\'t work well now, sorry';
    }
	else if(($source_len = strlen($_POST['source'])) > 100000){
		$errorMsg = 'Source is too long!';
	}

	if($errorMsg)
	{
    $errorMsg = urlencode($errorMsg);
		header("Location: submiterror.php?error=$errorMsg");
	    //确保重定向后，后续代码不会被执行
		exit;
	}
	if (isset($_POST['share']))
		$share = 'Y';
	else
		$share = 'N';
	if (isset($_POST['contest_id']) && !empty($_POST['contest_id']))
		$contest_id = $_POST['contest_id'];
	else
	    $contest_id = '0';

/************************************************/
	//这里将提交结果发送给中心server;
	$d = "\001";
	$submit_pack = $_POST['user_id'].$d.
				$_POST['pass'].$d.
				$_POST['problem_id'].$d.
				$contest_id.$d.
				$_POST['language'].$d.
				$source_len.$d.
				$share.$d.
				get_ip().$d.
				'N';

	if (submit($submit_pack, $_POST['source'])){
		if (empty($contest_id))
			header('Location: ../status/status.php');
		else
			header("Location: ../status/status.php?contest_id=$contest_id");
		$_SESSION['user_id'] = $_POST['user_id'];
		$_SESSION['password'] = $_POST['pass'];
	}
	else
		header("Location: submiterror.php?error=system is busy");
?>

<?php
function submit(&$submit_pack, &$source)
{
	$d="\001";
	$header = sprintf("%s%08d", "sm", strlen($submit_pack));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
		header('HTTP/1.1 404 Not Found');
		exit;
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($submit_pack)or die("send message failed");
	$tc->sendstr($source) or die ("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

?>

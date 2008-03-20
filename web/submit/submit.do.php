<?php

$error = null;
if (isset($_POST['submit'])){
	if(empty($_POST['user_id']) || empty($_POST['pass'])){
		$error = 'Invalid user ID or password';
	}
	else{
		$user_id = $_POST['user_id'];
		$pass = $_POST['pass'];
	}

	if(empty($_POST['problem_id'])){
		$error = 'please type your problem_id';
	}
	else{
		$problem_id = $_POST['problem_id'];
	}

	if(empty($_POST['language'])){
		$error = 'please type your language';
	}
	else{
		$language = $_POST['language'];
	}

	$source = $_POST['source'];
	if(strlen($source) < 20){
		$error = 'Source is too short';
	}
}
	if($error)
	{

		header("Location: submiterror.php?error=$error");
	    //确保重定向后，后续代码不会被执行
		exit;
	}

/************************************************/
	//这里将提交结果发送给中心server;
	echo $user_id.'<br>';
	echo $pass.'<br>';
	echo $problem_id.'<br>';
	echo $language.'<br>';
	echo $source.'<br>';
	echo $error.'<br>';


?>

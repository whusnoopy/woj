<?php
session_start();

function user_exist($user_id)
{
	if($user_id=='magiii') return true;
	return false;
}

function add_mail($to_user, $user_id, $title, $connent, $reply )
{
	return true;
}

function delete_mail($mail_id)
{
	return true;
}
?>

<?php


  $to_user = $_POST['to_user'];
  $title = $_POST['title'];
  $content = $_POST['content'];

  if(isset($_POST['type']))
	$type = $_POST['type'];
  else
    $type = $_GET['type'];
  if(isset($_POST['mail_id']))
	$mail_id = $_POST['mail_id'];
  else
    $mail_id = $_GET['mail_id'];


  if (isset($_SESSION['user_id']))
	  $user_id = $_SESSION['user_id'];
  else{
	  header("Location: mailerror.php?errorMsg=Please log in first.");
	  exit;
  }

  if ($type=='delete'){
	  delete_mail($mail_id);
	  header("Location: mailList.php");
	  exit;
  }

  if($to_user==null || strlen($to_user) ==0){
	  header("Location: mailerror.php?errorMsg=Receiver can't be null.");
	  exit;
  }
  if($title==null || strlen($to_user)==0){
	   header("Location: mailerror.php?errorMsg=Title can't be null.");
	  exit;
  }

  if(!user_exist($to_user)){
	  header("Location: mailerror.php?errorMsg=Receiver's id doesn't exist!");
	  exit;
  }

  if ($type=='send'){

	  if (add_mail($to_user, $user_id, $title, $connent, 0 )){
		  header("Location: mailSuccess.php");
		  exit;
	  }
	  else{
		  header("Location: mailerror.php?errorMsg=Send mail failed!");
		  exit;
	  }
  }
  else if($type=='reply'){
	  if (add_mail($to_user, $user_id, $title, $connent, 1 )){
		  header("Location: mailSuccess.php");
		  exit;
	  }
	  else{
		  header("Location: mailerror.php?errorMsg=Send mail failed!");
		  exit;
	  }
  }

 ?>









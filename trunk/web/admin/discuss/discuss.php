<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
?>
<?php
	include('../common/tcpclient.php');
	include('classes/discuss_list_t.php');

	if(isset($_GET['message_id']))
		$message_id = $_GET['message_id'];
	else{
		echo "the message does not exist!";
		exit;
	}

	$message = array();
	get_discuss_content($message_id, $message);

?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

<div id=main>
<table>
  <tbody>
  <tr class=tro>
    <th align=right width=100>Title:</th>
    <th align=left width=700><?php echo $message[1]; ?></th></tr>
  <tr class=tre>
    <td align=right><strong>Problem:</strong></td>
    <td align=left><?php echo $message[2]; ?></td></tr>
  <tr class=tre>
    <td align=right><strong>From:</strong></td>
    <td align=left><?php echo $message[4]; ?></td></tr>
  <tr class=tre>
    <td align=right><strong>Send Time:</strong></td>
    <td align=left><?php echo $message[5]; ?></td></tr>
  <tr class=tro valign=top>
    <td align=right><strong>Content:</strong></td>
    <td align=left>
      <div class=code><?php echo $message[6]; ?></div>
	</td>
  </tr>
   </tbody>
</table>

<br>
<hr>
<br>
<form action="discussAction.do.php" method=post>
  <input type="hidden" name="reply_id" value="<?php echo $message_id; ?>" />
  <input type="hidden" name="topic_id" value="<?php echo $message[0]; ?>" />
  <input type="hidden" name="type" value="reply" />
  <input type="hidden" name="problem_id" value="<?php echo $message[2]; ?>" />
  <table><tbody>
    <tr class=tre>
	  <td></td>
	  <td align=right><strong>Title:&nbsp;&nbsp;</strong></td>
	  <td align=left>&nbsp;&nbsp;<input size=80 name="title" value="Re:<?php echo $message[1]?>" /></td>
	  <td></td>
	</tr>

	<tr class=tro>
	  <td></td>
	  <td align=right valign=top><strong>Content:&nbsp;&nbsp;</strong></td>
	  <td align=left>&nbsp;&nbsp;<textarea name="content" rows=10 cols=80></textarea></td>
	  <td></td>
	</tr>
	<tr class=tre>
      <td colspan=4 align=center><input type=submit value=Send name=bs />&nbsp;&nbsp;
	  <input type=reset value=Reset name=rt /></td>
    </tr>
  </tbody></table>
</form>

<br>
<br>
</div>



<?php
	include('../include/tailer.php');
?>

<?php

////////title, problem_id, contest_id, user_id, date, content//////

function get_discuss_content($message_id, &$message)
{
		/////////////////////////////
//	$d="\001";
//	$recv = "11".$d."asdfsafdasdfasd".$d."1001".$d."1001".$d."magiii".$d."2008-03-26 12:12:12".$d."hello, flood!!!!!!!!!!";
//	$message = explode("\001", $recv);
//	return;
	//////////////////////////////

	if(empty($message_id)){
		$message = null;
		return;
	}

	$header = sprintf("%s%08d", "dc", strlen($message_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$message =  explode("\001", $recv);
	}
	else $message = null;
	$tc->close();
	return;
}
?>

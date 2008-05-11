<?php
	session_start();
	if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else{
		header("Location: ../user/login.php?origURL=../mail/mailList.php");
		exit;
	}
	include('../include/header.php');
	include('../common/tcpclient.php');
	if (isset($_GET['mail_id']))
		$mail_id = $_GET['mail_id'];
	else
		$mail_id = '0';

	$mail = array();
	get_mail_content($mail_id."\001".$user_id, $mail);

?>


  <div id="tt">Mail</div>
<?php
	include('../include/notice.php');
?>
  <div id="main">

  <table><tbody>
  <tr>
    <th width=80></th>
    <th width=100 align=right>Title:&nbsp;&nbsp;</th>
    <th width=700 align=left><?php echo $mail[2]; ?></th>
    <th width=70></th>
  </tr>
  <tr class=tro>
    <td></td>
    <td align=right><strong>From:&nbsp;&nbsp;</strong></td>
    <td align=left><?php echo $mail[0]; ?></td>
    <td></td>
  </tr>
  <tr class=tre>
    <td></td>
    <td align=right><strong>Send Time:&nbsp;&nbsp;</strong></td>
    <td align=left><?php echo $mail[3]; ?></td>
    <td></td>
  </tr>
  <tr class=tro valign=top>
    <td></td>
    <td align=right><strong>Content:&nbsp&nbsp;</strong></td>
    <td align=left><textArea name="content" rows=20 cols=80><?php echo $mail[4]; ?></textArea></td>
    <td></td>
  </tr>
  </tbody></table>

  <br />
  <span class="bt"><a href="mailList.php">Return</a></span>&nbsp;&nbsp;
  <span class="bt"><a href="replyMail.php?mail_id=<?php echo $mail_id; ?>&type=reply">Reply</a></span>&nbsp;&nbsp;
  <span class="bt"><a href="mailAction.do.php?mail_id=<?php echo $mail_id; ?>&type=delete">Delete</a></span>
  <br /><br />
  </div>

<?php
	include('../include/tailer.php');
?>

<?php

function get_mail_content($mail_id, &$mail)
{
	/////////////////////////////
/*	$d="\001";
	$recv = "mcje".$d."hello".$d."2008-03-25 12:12:12".$d."hello, world";
	$mail = explode("\001", $recv);
	return;
*/	//////////////////////////////

	if(empty($mail_id)){
		$mail = null;
		return;
	}
	$header = sprintf("%s%08d", "mc", strlen($mail_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($mail_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$mail =  explode("\001", $recv);
	}
	else $mail = null;
	$tc->close();
	return;
}
?>
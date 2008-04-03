<?php
	session_start();
	include('../include/header.php');

	if(!$user_id){
		header("Location: ../user/login.php?origURL=../mail/mailList.php");
		exit;
	}

	if (isset($_GET['mail_id']))
		$mail_id = $_GET['mail_id'];
	else
		$mail_id = '0';
	$mail = array();
	get_mail_content($maul_id, $mail);

?>

  <div id="tt">Reply Mail</div>

<?php
	include('../include/notice.php');
?>

  <div id="main">
    <form action="mailAction.do.php" method=post>
	  <input type="hidden" name="from_user" value="<?php echo $mail[0]; ?>" />
	  <input type="hidden" name="type" value="reply" />
	  <table><tbody>
<tr class=tro>
  <td width=80></td>
  <td width=100 align=right><strong>To:&nbsp;&nbsp;</strong></td>
  <td width=700 align=left>&nbsp;&nbsp;<input size=15 name="to_user" value="<?php echo $mail[0]; ?>" /> (Write Receiver's User Id here) </td>
  <td width=70></td>
</tr>
<tr class=tre>
  <td></td>
  <td align=right><strong>Title:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<input size=80 name="title" value="Re:<?php echo $mail[1]; ?>" /></td>
  <td></td>
</tr>
<tr class=tro>
  <td></td>
  <td align=right valign=top><strong>Content:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<textarea name="content" rows=20 cols=80>
  <?php echo "\n\n>".$mail[3]; ?>
  </textarea></td>
  <td></td>
</tr>
<tr class=tre>
  <td align=center colspan=4><input type=submit value=Send name=b1 /></td>
</tr>
      </tbody></table>
    </form>
  </div>

<?php
	include('../include/tailer.php');
?>

<?php

function get_mail_content($mail_id, &$mail)
{
	/////////////////////////////
	$d="\001";
	$recv = "mcje".$d."hello".$d."2008-03-25 12:12:12".$d."hello, world";
	$mail = explode("\001", $recv);
	return;
	//////////////////////////////

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
	$len = sscanf($recv, "%d");
	if($len > 0){
		$recv = $tc->recvstr($len);
		$mail =  explode("\001", $recv);
	}
	else $mail = null;
	$tc->close();
	return;
}
?>
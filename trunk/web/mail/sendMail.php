<?php
	session_start();
	if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else
		$user_id = '';

	if($user_id == ''){
		header("Location: ../user/login.php?origURL=../mail/sendMail.php");
		exit;
	}
	if(isset($_GET['to_user']))
		$to_user = $_GET['to_user'];
	else
		$to_user = '';
?>



<?php
	include('../include/header.php');
	echo '<div id="tt">Write Mail</div>';
	include('../include/notice.php');
?>

  <div id="main">
    <form action="mailAction.do.php" method=post>
      <input type="hidden" name="type" value="send" />
	  <table><tbody>
<tr class=tro>
  <td width=80></td>
  <td width=100 align=right><strong>To:&nbsp;&nbsp;</strong></td>
  <td width=700 align=left>&nbsp;&nbsp;<input size=15 name="to_user" value="<?php echo $to_user; ?>" /> (Write Receiver's User Id here) </td>
  <td width=70></td>
</tr>
<tr class=tre>
  <td></td>
  <td align=right><strong>Title:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<input size=80 name="title" /></td>
  <td></td>
</tr>
<tr class=tro>
  <td></td>
  <td align=right valign=top><strong>Content:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<textarea name="content" rows=20 cols=80></textarea></td>
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

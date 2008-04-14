<?php
	include('../include/header.php');
	echo '<div id="tt">Post Discuss </div>';
	include('../include/notice.php');
?>

<div id=main>
<br>
<form action="discussAction.do.php" method=post>
  <input type="hidden" name="type" value="send" />
  <input type="hidden" name="problem_id" value="<?php echo $_GET['pid']; ?>" />
  <table><tbody>
    <tr class=tre>
	  <td></td>
	  <td align=right><strong>Title:&nbsp;&nbsp;</strong></td>
	  <td align=left>&nbsp;&nbsp;<input size=80 name="title" value="<?php echo $message[1]?>" /></td>
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
</div>

<?php
	include('../include/tailer.php');
?>
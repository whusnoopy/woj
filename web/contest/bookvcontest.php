<?php
	include('../include/header.php');
	echo '<div id=tt>Book Virtual Contest</div>';
	include('../include/notice.php');
?>

<div id="main">
    <form action="bookvcontest.do.php" method="post">
    <table><tbody>
      <tr class=tro>
        <td width="350" align="right"><strong>User ID:&nbsp;</strong></td>
        <td width="550" align="left"><input name="user_id" type="text" value="" size="21" /></td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>Password:&nbsp;</strong></td>
        <td align="left"><input size="21" type="password" name="password" /></td>
      </tr>
      <tr class=tro>
        <td align="right"><strong>Title:&nbsp;</strong></td>
        <td align="left"><input size="60" type="text" name="title" /></td>
      </tr>
	  <tr class=tre>
        <td align="right"><strong>Description:&nbsp;</strong></td>
        <td align="left"><input size="60" type="text" name="description" value="" /></td>
      </tr>
      <tr class=tro>
        <td align="right"><strong>Start Time:&nbsp;</strong></td>
        <td align="left"><input size="21" type="text" name="start" />( yyyy-mm-dd hh:mm:ss, [utc+8] )</td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>End Time:&nbsp;</strong></td>
        <td align="left"><input size="21" type="text" name="end" />(at least 1 hour later, 30 minutes &le; length &le; 1 day)</td>
      </tr>
	  <tr class=tro>
        <td colspan=2 align=center>
        <input type="submit" value="Submit" name="submit" />&nbsp;&nbsp;
        <input type="reset" value="Reset" name="reset" />
        </td>
      </tr>
    </tbody></table>
    </form>
    <br /><br />
  </div>

<?php
	include('../include/tailer.php');
?>
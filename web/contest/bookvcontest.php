<?php
	include('../include/header.php');
	echo '<title>Book Virtual Contest</title>';
	echo '<div id=tt>Book Virtual Contest</div>';
	include('../include/notice.php');
?>

<div id="main">
    <form action="bookvcontest.do.php" method="post">
	<input type="hidden" name="type" value="V">
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
	    <td colspan=2 align="center">
	    <b>Contest problem list:</b>
	    </td>
      </tr>

<script language="javascript">
	var ch = new String("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	for (i=0; i<26; i++)
	if (i<3)
		document.writeln('<tr class=tre id="p'+i+'"><td align=right><strong>Problem '+ch.charAt(i)+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp;<input size=21 name="p'+i+'"></td></tr>');
	else
	    document.writeln('<tr class=tre id="p'+i+'"style="display:none"><td align=right><strong>Problem '+ch.charAt(i)+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp;<input size=21 name="p'+i+'"></td></tr>');
	document.forms[0].elements[7].value="1001";
</script>

	  <tr class=tro id="addmore">
	    <td colspan=2 align="center">
	    <a href="javascript:addProblem()">add more problems</a>
	    </td>
      </tr>

	  <tr class=tre>
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

<script language="javascript">
var id=3;
function addProblem()
{
	document.getElementById('p'+id).style.display = document.all ? "block" : "table-row";
	id++;
	if (id>25)
		document.getElementById('addmore').style.display = "none";
}
</script>
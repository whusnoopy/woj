<?php
	include('../include/header.php');
	echo '<title>Register</title>';
?>

  <div id="tt">New Account Register</div>
<?php
	include('../include/notice.php');
?>
  <div id="main">
    <form action="register.do.php" method="post" onsubmit="encode()">
    <table><tbody>
      <tr class=tro>
        <td width="400" align="right"><strong>User ID:&nbsp;</strong></td>
        <td align="left"><input name="user_id" type="text" value="" size="20" /></td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>Password:&nbsp;</strong></td>
        <td align="left"><input size="20" type="password" id="ps1" name="password" /></td>
      </tr>
      <tr class=tro>
        <td align="right"><strong>Re-type Password:&nbsp;</strong></td>
        <td align="left"><input size="20" type="password" id="ps2" name="repeatpassword" /></td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>Nick Name:&nbsp;</strong></td>
        <td align="left"><input size="40" type="text" name="nick" /></td>
      </tr>
	  <tr class=tro>
        <td align="right"><strong>Share code:&nbsp;</strong></td>
        <td align="left"><input size="40" type="checkbox" name="share_code" value=true checked />your source codes of a problem is open to anyone who has accepted it.</td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>School:&nbsp;</strong></td>
        <td align="left"><input size="40" type="text" name="school" /></td>
      </tr>
      <tr class=tro>
        <td align="right"><strong>Email:&nbsp;</strong></td>
        <td align="left"><input size="40" type="text" name="email" /></td>
      </tr>
	  <tr class=tre>
        <td align="right"><strong>Show email:&nbsp;</strong></td>
        <td align="left"><input size="40" type="checkbox" name="share_email" value=true checked />show your email to other</td>
      </tr>
      <tr class=tro>
        <td colspan=2 align=center>
        <input type="submit" value="Submit" name="submit" />&nbsp;
        <input type="reset" value="Reset" name="reset" />
        </td>
      </tr>
    </tbody></table>
    </form>
    <br /><br />
  </div>
  <script src="../common/md5.js"></script>
 <script language="javascript">
 function encode()
 {

	 document.getElementById('ps1').value = MD5(document.getElementById('ps1').value);
	 document.getElementById('ps2').value = MD5(document.getElementById('ps2').value);
	 return true;
 }
 </script>

<?php
	include('../include/tailer.php');
?>

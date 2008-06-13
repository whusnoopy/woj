<?php
    session_start();
	include('../include/header.php');
	echo '<title>Modify</title>';
?>

  <div id="tt">Modify Your Information</div>

<?php
	include ('../include/notice.php');
?>

  <div id="main">
    <form action="update.do.php"  method="post" onsubmit="encode()">
    <table><tbody>
      <tr class=tro>
        <td width="400" align="right"><strong>User ID:&nbsp;</strong></td>
        <td align="left"><input name="user_id" type="text" value="<?php echo $_SESSION['user_id'];?>" size="20" /></td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>Old Password:&nbsp;</strong></td>
        <td align="left"><input size="20" type="password" name="oldPass" /></td>
      </tr>
      <tr class=tro>
        <td align="right"><strong>New Password:&nbsp;</strong></td>
        <td align="left"><input size="20" type="password" id="ps1" name="newPass" /></td>
      </tr>
      <tr class=tre>
        <td align="right"><strong>Re-type Password:&nbsp;</strong></td>
        <td align="left"><input size="20" type="password" id="ps2" name="rptNewPass" /></td>
      </tr>
      <tr class=tro>
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
      <tr class=tre>
        <td colspan=2 align=center>
        <input type="submit" value="Submit" name="submit" />&nbsp;
        <input type="reset" value="Reset" name="reset" />
        </td>
      </tr>
    </tbody></table>
    </form>
    <br />
    <span class="ntc">Notice: Leave New Password and Re-type Password empty if you wanna keep it old.</span>
    <br /><br />
  </div>
<script src="../common/md5.js"></script>
 <script language="javascript">
 function encode(ps)
 {
	 document.getElementById('ps1').value = MD5(document.getElementById('ps1').value);
	 document.getElementById('ps2').value = MD5(document.getElementById('ps2').value);

	 return true;
 }
 </script>

<?php
	include('../include/tailer.php');
?>

<?php
	include('../include/header.php');
?>
<title>Login or Register</title>
  <div id="tt">Login or <a href=register.php>Register</a></div>
<?php
	include('../include/notice.php');
?>
  <div id="main">
<?php
  // Check is transfered here or not
	if (isset($_GET['origURL'])) {
		echo "<span class=cl>Please Login First!</span><br /><br />";
		$origURL = $_GET['origURL'];
	} else {
	  $origURL = '';
  }
	
  // Check error message
  if (isset($_GET['errorMsg']))
		echo "<span class=cl>$_GET[errorMsg]</span><br />";
?>
  <form name=loginform
        action="login.do.php?origURL=<?php echo $origURL; ?>"
        method=post
        onsubmit="encode(this.password.value)">
    <input type=hidden name=origURL value="<?php echo $origURL;?>"/>
    <table width=950><tbody>
      <tr class=tro>
        <td width=400 align=right><strong>Username:&nbsp;&nbsp;</strong></td>
        <td align=left><input name=user_id tabIndex=1 value="<?php echo $_COOKIE['user_id'];?>" size=20 maxlength=150 /></td>
      </tr>
      <tr class=tre>
        <td align=right><strong>Password:&nbsp;&nbsp;</strong></td>
        <td align=left><input name=password id=ps type=password tabIndex=2 value="<?php echo $_COOKIE['password']; ?>" size=20 maxLength=150 /></td>
      </tr>
      <tr class=tro>
        <td colspan=2 align=center>
          <input name=autoLogin type=checkbox class="radio" tabIndex=3 value=true <?php if(!empty($_COOKIE['user_id'])) echo "checked"; ?> />Remember Me
        </td>
      </tr>
      <tr class=tre>
        <td colspan=2 align=center>
          <input tabIndex=4 type=submit value=Login name=doLogin />&nbsp;
		      <input tabIndex=5 type=reset value=Reset name=doCancel />
        </td>
      </tr>
    </tbody></table>
  </form>
  <br/><span class=bt><a href=register.php>Register a new user id</a></span><br/><br/>
  </div>
<script src="../common/md5.js"></script>
<script language="javascript">
function encode(ps) {
  if (ps.length < 32)
    document.getElementById('ps').value = MD5(ps);
  return true;
}  
</script>

<?php
	include('../include/tailer.php');
?>

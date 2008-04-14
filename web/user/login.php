<?php
	include('../include/header.php');
?>

  <div id="tt">
    Login
  </div>

<?php
	include('../include/notice.php');
?>

  <div id="main">


<?php
	if (isset($_GET['origURL'])){
		echo "<span class=cl>Please Login First!</span>";
		$origURL = $_GET['origURL'];
	}
	if (isset($_GET['errorMsg']))
		echo "<span class=cl>$_GET[errorMsg] </span>";
?>
	<br>

    <form name=loginform action="login.do.php?origURL=<?php echo $origURL; ?>" method=post>
      <input type=hidden name=origURL value=<?php echo $_GET['origURL'];?>/>
      <table width=950><tbody>
        <tr class=tro>
          <td width=400 align=right><strong>Username:&nbsp;&nbsp;</strong></td>
          <td align=left><input name=user_id tabIndex=1 value="<?php echo $_COOKIE['user_id'];?>" size=20 maxlength=150 /></td>
        </tr>
        <tr class=tre>
          <td align=right><strong>Password:&nbsp;&nbsp;</strong></td>
          <td align=left><input name=password type=password tabIndex=2 value="<?php echo $_COOKIE['password']; ?>" size=20 maxLength=150 /></td>
        </tr>
        <tr class=tro>
          <td colspan=2 align=center>
          <input name=autoLogin type=checkbox class="radio" tabIndex=3 value=true <?php !empty($_COOKIE['user_id']) ? 'checked': ''; ?> />Remember Me
          </td>
        </tr>
        <tr class=tre>
          <td colspan=2 align=center>
          <input tabIndex=4 type=submit value=Login name=doLogin />&nbsp;
		  <input tabIndex=5 type=submit value=Cancel name=doCancel />
          </td>
        </tr>
      </tbody></table>
    </form>
  </div>

<?php
	include('../include/tailer.php');
?>

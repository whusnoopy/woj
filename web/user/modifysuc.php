<?php
	include('../include/header.php');
?>

  <div id="tt">Modified Your Information Successful</div>
<?php
	include('../include/notice.php');
?>

  <div id="main">
    <table><tbody>
  <tr class=tro>
    <td width="400" align="right"><strong>User ID:&nbsp;</strong></td>
    <td align="left"><?php echo $_GET['user_id']; ?></td>
  </tr>
  <tr class=tre>
    <td align="right"><strong>Nick Name:&nbsp;</strong></td>
    <td align="left"><?php echo $_GET['nick']; ?></td>
  </tr>
  <tr class=tro>
    <td align="right"><strong>School:&nbsp;</strong></td>
    <td align="left"><?php echo $_GET['school']; ?></td>
  </tr>
  <tr class=tre>
    <td align="right"><strong>Email:&nbsp;</strong></td>
    <td align="left"><?php echo $_GET['email']; ?></td>
  </tr>
    </tbody></table>
    <br />
    <span class="bt"><a href="../index.php">Back to HomePage</a></span>
    <br /><br />
  </div>

<?php
	include('../include/tailer.php');
?>

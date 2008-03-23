
<?php
  session_start();
  include ('../include/header.php');

  if(isset($_GET['problem_id'])){
    $problem_id = $_GET['problem_id'];
 }
 else{
    $problem_id = '';
  }
  $user_id = $_SESSION['user_id'];
  $pass = $_SESSION['password'];
?>

  <div id="tt">Submit</div>
<?php
    include ('../include/notice.php');
?>

  <div id="main">
    <form action="submit.do.php" method=post>
    <table><tbody>
      <tr>
        <th width=80></th>
        <th colspan=2>Fill in your information and paste your source here</th>
        <th width=80></th>
      </tr>
      <tr class=tro>
        <td></td>
        <td align="right" width=100><strong>User ID:&nbsp;&nbsp;</strong></td>
        <td align="left"><input id=user_id maxLength=20 size=25 name=user_id value="<?php echo $user_id; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><strong>Password:&nbsp;&nbsp;</strong></td>
        <td align="left"><input id=pass type=password maxLength=20 size=25 name=pass value="<?php echo $pass; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tro>
        <td></td>
        <td align="right"><strong>Problem ID:&nbsp;&nbsp;</strong></td>
        <td align="left"><input id=problem_id maxLength=5 size=10 name=problem_id value="<?php echo $problem_id ?>" /></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><strong>Language:&nbsp;&nbsp;</strong></td>
        <td align="left">
          <select id=language size=1 name=language>
          <option value=0>GCC</option><option value=1 selected>G++</option><option value=2>Java</option><option value=3>Pascal</option>
          </select>
        </td>
        <td></td>
      </tr>
      <tr class=tro valign=top>
        <td></td>
        <td align="right"><strong>Source:&nbsp;&nbsp;</strong></td>
        <td align="left"><textarea id=source name=source rows=20 cols=80></textarea></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td colSpan=4 align=center><input type=submit value=Submit name=submit />&nbsp;<input type=reset value=Reset /></td>
      </tr>
    </tbody></table>
    </form>
  </div>

<?php
    include ('../include/tailer.php');
?>


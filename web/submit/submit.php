<?php
  session_start();
  include ('../include/header.php');
  echo '<title>Submit</title>';

  if(isset($_GET['problem_id']))
    $problem_id = $_GET['problem_id'];
  else
    $problem_id = '';

  if(isset($_GET['contest_id']))
    $contest_id = $_GET['contest_id'];
  else
    $contest_id = '0';

  $user_id = $_SESSION['user_id'];
  $pass = $_SESSION['password'];
?>

  <div id="tt">Submit</div>
<?php
    include ('../include/notice.php');
?>
<script src="../common/md5.js"></script>
<script language="javascript">
  function validateFormData(){
    try{
    var userId = document.getElementById("userId");
    var pass = document.getElementById("ps");
    var problemId = document.getElementById("problemId");
    var source = document.getElementById("source");
    if(userId.value.length == 0 || pass.value.length == 0){
      alert("Invalid userId or password");
      userId.focus();
      return false;
    }else if(problemId.value.length == 0){
      alert("Invalid problem id");
      problemId.focus();
      return false;
    }else if(source.value.length == 0){
      alert("Please paste your source before submit");
      source.focus();
      return false;
    }else if(source.value.length > 20000){
      alert("Source is too long");
      source.focus();
      return false;
    }
    if(pass.value.length < 32)
        pass.value = MD5(pass.value);
    var form1 = document.getElementById("form1");
    try{ form1.submit();}
    catch(e){form1.submit.click();}
    }catch(e1){alert(e1);}
  }
</script>
  <div id="main">
    <form id="form1" action="submit.do.php" method="post">
	<input type="hidden" name="contest_id" value="<?php echo $contest_id;?>"/>
    <table><tbody>
      <tr>
        <th width=80></th>
        <th colspan=2>Fill in your information and paste your source here</th>
        <th width=80></th>
      </tr>
      <tr class=tro>
        <td></td>
        <td align="right" width=100><b>User ID:&nbsp;</b></td>
        <td align="left"><input maxLength=20 size=25 id="userId" name="user_id" value="<?php echo $user_id; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><b>Password:&nbsp;</b></td>
        <td align="left"><input type=password id=ps maxLength=150 size=25 name="pass" value="<?php echo $pass; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tro>
        <td></td>
        <td align="right"><b>Problem ID:&nbsp;</b></td>
        <td align="left">
          <input maxLength=5 size=10 id="problemId" name="problem_id" value="<?php
          if(!empty($problem_id)){
            echo $problem_id;
          }else{
            echo $_SESSION['problem_id'];
          }
          ?>"/></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><b>Language:&nbsp;</b></td>
        <td align="left">
          <select size=1 name="language">
          <option value=0>GCC</option><option value=1 selected>G++</option><option value=2>Java</option><option value=3>Pascal</option>
          </select>
        </td>
        <td></td>
      </tr>
	  <tr class=tro>
		<td></td>
		<td align="right"><b>Share Code:&nbsp;</b></td>
		<td align="left"><input type="checkbox" name="share" checked/><td>
		<td></td>
	  </tr>
      <tr class=tro valign="top">
        <td></td>
        <td align="right"><b>Source:&nbsp;</b></td>
        <td align="left">
          <textarea style="width:700px; height:500px; wrap: soft" id="source" name="source"><?php echo htmlspecialchars($_SESSION['source']); ?></textarea></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td colSpan=4 align=center>
        <input type="button" value="Submit" onclick="validateFormData()"/>
        &nbsp;<input type="reset" value="Reset" />
        <input type="submit" value="Submit" name="submit" style="display:none"/></td>
      </tr>
    </tbody></table>
    </form>
  </div>
<?php
    include ('../include/tailer.php');
?>


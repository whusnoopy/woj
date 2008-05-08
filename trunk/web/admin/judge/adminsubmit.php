<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:../login.php?errorMsg=please login first!');
		exit;
	}
?>
<?php
  if(isset($_GET['problem_id'])){
    $problem_id = $_GET['problem_id'];
 }
 else{
    $problem_id = '';
  }
  $user_id = $_SESSION['user_id'];
  $pass = $_SESSION['password'];
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

  <div id="tt">Submit</div>

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
        <td align="right" width=100><b>User ID:&nbsp;&nbsp;</b></td>
        <td align="left"><input maxLength=20 size=25 name="user_id" value="<?php echo $user_id; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><b>Password:&nbsp;&nbsp;</b></td>
        <td align="left"><input type=password maxLength=20 size=25 name="pass" value="<?php echo $pass; ?>" /></td>
        <td></td>
      </tr>
      <tr class=tro>
        <td></td>
        <td align="right"><b>Problem ID:&nbsp;&nbsp;</b></td>
        <td align="left"><input maxLength=5 size=10 name="problem_id" value="<?php echo $problem_id ?>" /></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td></td>
        <td align="right"><b>Language:&nbsp;&nbsp;</b></td>
        <td align="left">
          <select size=1 name="language">
          <option value=0>GCC</option><option value=1 selected>G++</option><option value=2>Java</option><option value=3>Pascal</option>
          </select>
        </td>
        <td></td>
      </tr>
	  <tr class=tro>
		<td></td>
		<td align="right"><b>Share code:&nbsp;&nbsp</b></td>
		<td align="left"><input type="checkbox" name="share" checked/><td>
		<td></td>
	  </tr>
      <tr class=tro valign="top">
        <td></td>
        <td align="right"><b>Source:&nbsp;&nbsp;</b></td>
        <td align="left"><textarea id="source" name="source" rows=20 cols=80></textarea></td>
        <td></td>
      </tr>
      <tr class=tre>
        <td colSpan=4 align=center><input type="submit" value="Submit" name="submit" />&nbsp;<input type="reset" value="Reset" /></td>
      </tr>
    </tbody></table>
    </form>
  </div>

	<div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>
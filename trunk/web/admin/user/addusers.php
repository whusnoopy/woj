<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
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
    <a href="problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>


<div id="tt">
    Add Series Users
</div>

<div id="main" align="left">
  <form action="addUsersAction.do.php" method=post>
	<table width=950><tbody>
        <tr class=tro>
          <td width=400 align=right><input type="checkbox" name="file" onclick="fromfile(this.form)"><b>import from file:&nbsp;&nbsp;</b></td>
          <td align=left><input type="file" name="filename" disabled/></td>
        </tr>
        <tr class=tre>
          <td align=right><b>User name Prefix:&nbsp;&nbsp;</b></td>
          <td align=left><input type="text" name="prefix" value="WHU_"> </td>
        </tr>
		 <tr class=tre>
          <td align=right><b>User name Suffix:&nbsp;&nbsp;</b></td>
          <td align=left><input type="checkbox" name="suffix"> (use number as suffix)</td>
        </tr>
		<tr class=tro>
		  <td align=right><b>User name:&nbsp;&nbsp;</b></td>
		  <td align=left><textarea name="username" rows=10 cols=60>One name a line</textarea><td>
		<tr>
        <tr class=tre>
          <td colspan=2 align=center>
          <input tabIndex=4 type="submit" value="Submit" />&nbsp;
		  <input tabIndex=5 type="reset" value="Reset"  />
          </td>
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

<script language="javascript">
function fromfile(form)
{
	if (form.file.checked){
		form.filename.disabled = false;
		form.username.disabled= true;
	}
	else{
		form.filename.disabled = true;
		form.username.disabled = false;
	}
}
//-->
</script>
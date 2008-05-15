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


  <div id="tt">User Details</div>


  <div id="main">
    <form action="updateByAdmin.do.php"  method="post">
    <table><tbody>
      <tr class=tro>
        <td width="400" align="right"><strong>User ID:&nbsp;</strong></td>
        <td align="left"><input name="user_id" type="text" value="" size="20" /></td>
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

<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
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
    <a href="contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

  <div id="tt">
    Add A New Contest
  </div>

<script language="javascript">
<!--
function makesure()
{
	return confirm("Make sure your submit!");
}
//-->
</script>

  <div id="main">
<form action="addContestAction.do.php" method=post onSubmit="return makesure();">
  <table><tbody>
    <tr>
      <th colspan=4 align=center>Fill the Informations</th>
    </tr>
    <tr class=tro>
      <td width=100></td>
	  <td width=150 align=right><strong>Title:</strong>&nbsp;&nbsp;</td>
      <td width=600 align=left>&nbsp;&nbsp;<input size=75 name="title" /></td>
      <td width=100></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Type:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;
        <select name="type">
          <option value="N" selected>Public</option>
		  <option value="P">Private</option>
		  <option value="A" >Award</option>
        </select>
      </td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Description:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="description" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Start Time:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input size=21 name="start">( yyyy-mm-dd hh:mm:ss, [utc+8] )</td>
      <td></td>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>End Time:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input size=21 name="end"> </td>
      <td></td>
    </tr>
	<tr class=tre>
	  <td colspan=4 align="center">
	  <b>Problem list:</b>
	  </td>
    </tr>

<script language="javascript">
	var ch = new String("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	for (i=0; i<26; i++)
	if (i<3)
		document.writeln('<tr class=tro id="p'+i+'"><td></td><td align=right><strong>Problem '+ch.charAt(i)+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp;<input size=21 name="p'+i+'"></td><td></td></tr>');
	else
	    document.writeln('<tr class=tro id="p'+i+'"style="display:none"><td></td><td align=right><strong>Problem '+ch.charAt(i)+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp;<input size=21 name="p'+i+'"></td><td></td></tr>');
</script>

	<tr class=tre id="addmore">
	  <td colspan=4 align="center">
	  <a href="javascript:addProblem()">add more problems</a>
	  </td>
    </tr>
    <tr class=tro>
      <td colspan=4 align=center>
        <input type="submit" name="submit" value="Submit" />&nbsp;
        <input name="reset" type="reset" value="Reset" />
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
var id=3;
function addProblem()
{
	document.getElementById('p'+id).style.display = document.all ? "block" : "table-row";
	id++;
	if (id>25)
		document.getElementById('addmore').style.display = "none";
}
</script>

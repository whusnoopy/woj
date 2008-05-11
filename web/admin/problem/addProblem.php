<?php
	session_start();
	if (empty($_SESSION['user_id'])){
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
    <a href="problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>


<div id="tt">Add a new problem</div>


<script language="javascript">
<!--
function makesure(msg)
{
	return confirm(msg);
}

function spjtest(form)
{
	if (form.spj.checked)
	    form.spj_file.disabled=false;
	else
		form.spj_file.disabled=true;
}
//-->
</script>

<div id="main">
<form  name="form" method=post enctype="multipart/form-data"  onReset="return makesure('drop it?');">
  <input type="hidden" name="type" value="add">
  <table><tbody>
    <tr>
      <th colspan=4 align=center>Fill the Informations</th>
    </tr>
    <tr class=tro>
      <td width=100></td>
	  <td width=150 align=right><strong>Problem ID:</strong>&nbsp;&nbsp;</td>
      <td width=600 align=left>&nbsp;&nbsp;New Problem</td>
      <td width=100></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Title:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name="title" size=75 /></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name="time_limit" value=1000 size=15 />MS</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Case Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name="case_time_limit" value=30000 size=15 />MS</td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Memory Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name="memory_limit" value=65536 size=15 />KB</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Description:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<div id="text"><textarea name="description" id="description" rows=10 cols=80></textarea></div>
	  <div id="html" style=display:none></div>
	  <input type="button"  id="change" value="View" onclick="changemodel();">
	  </td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="input" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="output" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Sample Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="sample_input" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Sample Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="sample_output" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Hint:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name="hint" rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Source:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name="source" size=75 /></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Related Contest:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;gen by system</td>
      <td></td>
    </tr>
	<tr class=tre>
	  <td></td>
	  <td align=right><strong>Special judge:</strong>&nbsp;&nbsp;</td>
	  <td align=left>&nbsp;&nbsp;
	  <input type="checkbox" name="spj" class="radio" tabIndex=3 value="Y" onclick="spjtest(this.form)"/>
	  </td>
	  <td></td>
	</tr>


	<tr class=tro><td colspan=4></td></tr>
	<tr class=tro>
      <td colspan=4 align=center><b>Picture list:</b></td>
    </tr>

<script language="javascript">
	for (i=1; i<=5; i++)
	{
		if (i<=1)
		    document.writeln('<tr class=tre id="pic'+i+'"><td></td><td align=right><strong>Picture'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="pic'+i+'" size=60 onchange="add_img_bz(this.form, this.value)"/> </td><td></td></tr>');
		else
			document.writeln('<tr class=tre id="pic'+i+'" style="display:none"><td></td><td align=right><strong>Picture'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="pic'+i+'" size=60 onchange="add_img_bz(this.form, this.value)"/></td><td></td></tr>');
	}
</script>

	<tr class=tro id="add_more_pic_file">
	  <td colspan=4 align="center">
	  <a href="javascript:addFile('pic')">add more picture files</a>
	  </td>
    </tr>




	<tr class=tre><td colspan=4></td></tr>
	<tr class=tre>
      <td colspan=4 align=center><b>Common files list:</b></td>
    </tr>
<script language="javascript">
	for (i=1; i<=5; i++)
	{
		if (i<=1)
		    document.writeln('<tr class=tro id="commonfile'+i+'"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="common'+i+'" size=60/></td><td></td></tr>');
		else
			document.writeln('<tr class=tro id="commonfile'+i+'" style="display:none"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="common'+i+'" size=60/></td><td></td></tr>');
	}
</script>

	<tr class=tre id="add_more_common_file">
	  <td colspan=4 align="center">
	  <a href="javascript:addFile('common')">add more common files</a>
	  </td>
    </tr>



	<tr class=tro><td colspan=4> </td></tr>
	<tr class=tro>
      <td colspan=4 align=center><b>Input/output files list:</b></td>
    </tr>
<script language="javascript">
	for (i=1; i<=10; i++)
	{
		if (i<=3)
		    document.writeln('<tr class=tre id="file'+i+'"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; input'+i+': <input type="file" name="in'+i+'" size=30/>&nbsp;&nbsp; output'+i+':<input type="file" name="out'+i+'" size=30/></td><td></td></tr>');
		else
			document.writeln('<tr class=tre id="file'+i+'" style="display:none"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; input'+i+': <input type="file" name="in'+i+'" size=30/>&nbsp;&nbsp; output'+i+':<input type="file" name="out'+i+'" size=30/></td><td></td></tr>');
	}
</script>

	<tr class=tro id="add_more_inout_file">
	  <td colspan=4 align="center">
	  <a href="javascript:addFile('file')">add more input/output files</a>
	  </td>
    </tr>
	<tr class=tro>
	  <td></td>
	  <td align=right><strong>Special judge file:</strong>&nbsp;&nbsp;</td>
	  <td align=left>&nbsp;&nbsp; <input type="file" name="spj_file" disabled/>&nbsp;<font color=red>if not special judge just leave it out</font></td>
	  <td></td>
	</tr>


    <tr class=tre>
      <td colspan=4 align=center>
		<input type="button" value="Preview" onclick="onShow(this.form)"/>&nbsp
		<input type="button" value="Submit"  onclick="onSubmit(this.form)"/>&nbsp;
        <input type="reset" value="Reset" />
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

var src='';

function changemodel()
{
	if (em("text").style.display == "none")
	{
		em("html").style.display = "none";
		em("text").style.display = "block";
		em("change").value = "View";
	}
	else
	{
		em("text").style.display = "none";
		em("html").style.display = "block";
		em("html").innerHTML = em("description").value;
		em("change").value = "Edit";
	}
}

function em(element)
{
	return document.getElementById(element);
}

var fileid=4;
var picid=2;
var commonid=2;
function addFile(bz)
{
	if (bz=='file'){
		em('file'+fileid).style.display = document.all ? "block" : "table-row";
		fileid++;
		if (fileid>10)
			em('add_more_inout_file').style.display = "none";
	}
	else if (bz=='pic'){
		em('pic'+picid).style.display = document.all ? "block" : "table-row";
		picid++;
		if (picid>5)
			em('add_more_pic_file').style.display = "none";
	}
	else if (bz=='common'){
		em('commonfile'+commonid).style.display = document.all ? "block" : "table-row";
		commonid++;
		if (commonid>5)
			em('add_more_common_file').style.display = "none";
	}
}

function onShow(form)
{
	form.action="preview.php";
	form.target="_blank";
	form.submit();
}

function onSubmit(form)
{
	if (confirm('Submit it now?')){
		form.action="addProblemAction.do.php";
		form.target="_self";
		form.submit();
	}
}

function add_img_bz(form, value)
{
	form.description.value += ('  <img src="' + value + '"/>');
}
</script>
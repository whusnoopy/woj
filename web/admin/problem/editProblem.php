<?php
	session_start();
	if (empty($_SESSION['user_id'])){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
	$problem_id = $_GET['problem_id'];
	$problem = array();
	getProblemDetails($problem_id, $problem);
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


<div id="tt">Edit problem <?php echo $problem_id; ?></div>


<script language="javascript">
<!--
function makesure(msg)
{
	return confirm(msg);
}

function spjtest(form)
{
	if (form.spj.checked && form.updatefiles.checked)
	    form.spj_file.disabled=false;
	else
		form.spj_file.disabled=true;
}

function upfile(form)
{
	if (form.updatefiles.checked){
		form.pic1.disabled = false;
		form.pic2.disabled = false;
		form.pic3.disabled = false;
		form.pic4.disabled = false;
		form.pic5.disabled = false;
		form.common1.disabled = false;
		form.common2.disabled = false;
		form.common3.disabled = false;
		form.common4.disabled = false;
		form.common5.disabled = false;
		form.in1.disabled = false;
		form.in2.disabled = false;
		form.in3.disabled = false;
		form.in4.disabled = false;
		form.in5.disabled = false;
		form.in6.disabled = false;
		form.in7.disabled = false;
		form.in8.disabled = false;
		form.in9.disabled = false;
		form.in10.disabled = false;
		form.out1.disabled = false;
		form.out2.disabled = false;
		form.out3.disabled = false;
		form.out4.disabled = false;
		form.out5.disabled = false;
		form.out6.disabled = false;
		form.out7.disabled = false;
		form.out8.disabled = false;
		form.out9.disabled = false;
		form.out10.disabled = false;
	}
	else{
		form.pic1.disabled = true;
		form.pic2.disabled = true;
		form.pic3.disabled = true;
		form.pic4.disabled = true;
		form.pic5.disabled = true;
		form.common1.disabled = true;
		form.common2.disabled = true;
		form.common3.disabled = true;
		form.common4.disabled = true;
		form.common5.disabled = true;
		form.in1.disabled = true;
		form.in2.disabled = true;
		form.in3.disabled = true;
		form.in4.disabled = true;
		form.in5.disabled = true;
		form.in6.disabled = true;
		form.in7.disabled = true;
		form.in8.disabled = true;
		form.in9.disabled = true;
		form.in10.disabled = true;
		form.out1.disabled = true;
		form.out2.disabled = true;
		form.out3.disabled = true;
		form.out4.disabled = true;
		form.out5.disabled = true;
		form.out6.disabled = true;
		form.out7.disabled = true;
		form.out8.disabled = true;
		form.out9.disabled = true;
		form.out10.disabled = true;
	}
}

//-->
</script>

<div id="main">
<form name="form" method=post enctype="multipart/form-data" onReset="return makesure('Drop it?');">
  <input type="hidden" name="type" value="edit">
  <input type="hidden" name="problem_id" value="<?php echo $problem_id?>">
  <table><tbody>
    <tr>
      <th colspan=4 align=center>Fill the Informations</th>
    </tr>
    <tr class=tro>
      <td width=100></td>
	  <td width=150 align=right><strong>Problem ID:</strong>&nbsp;&nbsp;</td>
      <td width=600 align=left>&nbsp;&nbsp;<?php echo $problem_id; ?></td>
      <td width=100></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Title:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=title size=75 value="<?php echo $problem[0]; ?>"/></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=time_limit size=15 value="<?php echo $problem[9]; ?>"/>MS</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Case Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=case_time_limit  ize=15 value="<?php echo $problem[10]; ?>"/>MS</td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Memory Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=memory_limit size=15 value="<?php echo $problem[11]; ?>"/>KB</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Description:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=description rows=10 cols=80 ><?php echo $problem[1]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=input rows=5 cols=80><?php echo $problem[2]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=output rows=5 cols=80><?php echo $problem[3]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Sample Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=sample_input rows=5 cols=80 ><?php echo $problem[4]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Sample Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=sample_output rows=5 cols=80 ><?php echo $problem[5]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Hint:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=hint rows=5 cols=80 ><?php echo $problem[6]; ?></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Source:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=source size=75 value="<?php echo $problem[7]; ?>"/></td>
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
	  <?php
	  if ($problem[15] == 'Y')
	    echo '<input type="checkbox" name="spj" class="radio" tabIndex=3 value="Y" cheched onclick="spjtest(this.form)"/>';
	  else
	    echo '<input type="checkbox" name="spj" class="radio" tabIndex=3 value="N" onclick="spjtest(this.form)"/>';
	  ?>
	  </td>
	  <td></td>
	</tr>

    <tr class=tro>
	   <td></td>
	   <td align=right><b>Update Files:</b>&nbsp;&nbsp;</td>
	   <td align=left>&nbsp;&nbsp;<input type="checkbox" name="updatefiles" class="ratio" tabIndex=3 value ="Y" onclick="upfile(this.form)"/></td>
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
		    document.writeln('<tr class=tre id="pic'+i+'"><td></td><td align=right><strong>Picture'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="pic'+i+'" size=60 disabled/></td><td></td></tr>');
		else
			document.writeln('<tr class=tre id="pic'+i+'" style="display:none"><td></td><td align=right><strong>Picture'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="pic'+i+'" size=60 disabled/></td><td></td></tr>');
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
		    document.writeln('<tr class=tro id="commonfile'+i+'"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="common'+i+'" size=60 disabled/></td><td></td></tr>');
		else
			document.writeln('<tr class=tro id="commonfile'+i+'" style="display:none"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; <input type="file" name="common'+i+'" size=60 disabled/></td><td></td></tr>');
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
		    document.writeln('<tr class=tre id="file'+i+'"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; input'+i+': <input type="file" name="in'+i+'" size=30 disabled/>&nbsp;&nbsp; output'+i+':<input type="file" name="out'+i+'" size=30 disabled/></td><td></td></tr>');
		else
			document.writeln('<tr class=tre id="file'+i+'" style="display:none"><td></td><td align=right><strong>File'+i+':</strong>&nbsp;&nbsp;</td><td align=left>&nbsp;&nbsp; input'+i+': <input type="file" name="in'+i+'" size=30 disabled/>&nbsp;&nbsp; output'+i+':<input type="file" name="out'+i+'" size=30 disabled/></td><td></td></tr>');
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
var fileid=4;
var picid=2;
var commonid=2;
function addFile(bz)
{
	if (bz=='file'){
		document.getElementById('file'+fileid).style.display = document.all ? "block" : "table-row";
		fileid++;
		if (fileid>10)
			document.getElementById('add_more_inout_file').style.display = "none";
	}
	else if (bz=='pic'){
		document.getElementById('pic'+picid).style.display = document.all ? "block" : "table-row";
		picid++;
		if (picid>5)
			document.getElementById('add_more_pic_file').style.display = "none";
	}
	else if (bz=='common'){
		document.getElementById('commonfile'+commonid).style.display = document.all ? "block" : "table-row";
		commonid++;
		if (commonid>5)
			document.getElementById('add_more_common_file').style.display = "none";
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
		form.action="editProblemAction.do.php";
		form.target="_self";
		form.submit();
	}
}
</script>

<?php
function getProblemDetails($problem_id, &$problem)
{
	$d = "\001";
	$header = sprintf("%s%08d", "mp", strlen($problem_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$problem = explode($d, $recv);
	}
	else
		$problem = null;
	$tc->close();
}
?>
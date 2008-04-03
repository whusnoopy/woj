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



<div id="main">
<form action="addProblemAction.do.php" method=post enctype="multipart/form-data">
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
      <td align=left>&nbsp;&nbsp;<input name=title size=75 /></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=time_limit value=1000 size=15 />MS</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Case Time Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=case_time_limit value=30000 size=15 />MS</td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Memory Limit:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=memory_limit value=65536 size=15 />KB</td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Description:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=description rows=10 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=input rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=output rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Sample Input:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=sample_input rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Sample Output:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=sample_output rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Hint:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<textarea name=hint rows=5 cols=80></textarea></td>
      <td></td>
    </tr>
    <tr class=tre>
      <td></td>
	  <td align=right><strong>Source:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;<input name=source size=75 /></td>
      <td></td>
    </tr>
    <tr class=tro>
      <td></td>
	  <td align=right><strong>Related Contest:</strong>&nbsp;&nbsp;</td>
      <td align=left>&nbsp;&nbsp;gen by system</td>
      <td></td>
    </tr>
	<tr class=tro>
	  <td></td>
	  <td align=right><strong>Special judge:</strong>&nbsp;&nbsp;</td>
	  <td align=left>&nbsp;&nbsp;
	  <input type="checkbox" name="spj" class="radio" tabIndex=3 value="Y"/>
	  </td>
	  <td></td>
	</tr>
	<tr class=tre>
	  <td></td>
	  <td align=right><strong>Picture:</strong>&nbsp;&nbsp;</td>
	  <td align=left>&nbsp;&nbsp; <input type="file" name="pic"/></td>
	  <td></td>
	</tr>
<?php
	for ($i=1; $i<=5; $i++)
	{
		echo '<tr class=tro>';
	    echo '<td></td>';
	    echo "<td align=right><strong>File$i:</strong>&nbsp;&nbsp;</td>";
	    echo "<td align=left>&nbsp;&nbsp; input$i: <input type=\"file\" name=\"in$i\"/>";
		echo "&nbsp;&nbsp; output$i:<input type=\"file\" name=\"out$i\"/></td>";
	    echo '<td></td>';
	    echo '</tr>';
	}

?>
	<tr class=tre>
	  <td></td>
	  <td align=right><strong>Special judge file:</strong>&nbsp;&nbsp;</td>
	  <td align=left>&nbsp;&nbsp; <input type="file" name="spj_file"/>&nbsp;<font color=red>if not special judge just leave it out</font></td>
	  <td></td>
	</tr>


    <tr class=tre>
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
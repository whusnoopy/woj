<?php
	session_start();
	if (!isset($_SESSION['user_id']) && $_SESSION['access'] != 'root'){
		header('Location: login.php');
		exit;
	}

	include('../common/tcpclient.php');

	if (isset($_POST['submit'])){
		addStudents($_POST['students']);
		echo '<br><a href="javascript:history.back()">Back</a>&nbsp;&nbsp;';
		echo '<a href="index.php">Home</a>';
		exit;
	}
?>

<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Programming Management</title>
  <link href="../style/t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Programming Management</b><br>
	<b>Add series students</b><br>
	<hr><br>
	<form name="form" action="addstudents.php" method=post>
		<textarea name="students" rows=20 cols=80 text>  One line a student's infomation, include student_id,ID Card number,realname,grade, separated by ','. For example:
200431500218,430121198804214561,张三,2004
200431500311,430121198705221234,李四,2004</textarea><br>
		<div id="stable" style="display:none"></div>
		<input type="button" name="preview" value="Preview" onclick="show(this.form)"/>
		<input type="submit" name="submit" value="Submit"/>
		<input type="reset" value="Reset"/>
	</form>
	</div>
</body>
</html>

<script language="javascript">
function show(form)
{
	if (form.preview.value == "Preview"){
		var rows=form.students.value.split("\r\n");
		form.students.style.display = "none";
		var tb = "<table border=1><th>学号</th><th>身份证号</th><th>姓名</th><th>年级</th>";
		for (i=0; i<rows.length; i++){
			tb += '<tr>';
			var cols = rows[i].split(",");
			for (j=0; j<cols.length; j++)
				tb += '<td>' + cols[j] + '</td>';
			tb += '</tr>'
		}
		tb += '</table>';
		document.getElementById("stable").innerHTML = tb;
		document.getElementById("stable").style.display = "block";
		form.preview.value = "Return";
	}
	else{
		form.students.style.display = "block";
		document.getElementById("stable").style.display = "none";
		form.preview.value = "Preview";
	}
}
</script>

<?php

function genPassword()
{
	$words = '1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$pass= '';
	//no need srand() from php4.2.0
	for ($i=0; $i<8; $i++)
		$pass .= $words{rand(0,61)};
	return $pass;
}

function addStudents($str)
{
	$students = explode("\r\n", $str);

	forEach ($students as $student){
		$info = explode(",", $student);
		if (count($info) != 4 ){
			echo 'student information missing : '.$student.'<br>';
			return 0;
		}
	}

	$d = "\001";
	$n = count($students);
	$pass = array();
	$valid = array();
	$message = $n;
	//student_id,password,ID Card number,realname,grade,class
	for ($i=0; $i<$n; $i++){
		$info = explode(",", $students[$i]);
		$message .= ($d.$info[0]);
		$pass[$i] = genPassword();
		$message .= ($d.$pass[$i]);
		for ($j=1; $j<4; $j++)
			$message .= ($d.$info[$j]);
		$message .= $d.'0';
	}

	$header = sprintf("%s%08d", "es", strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$valid = explode($d, $recv);
	}
	else
		$valid = null;
	$tc->close();

	if (!$valid){
		echo 'Add series students failed!<br>';
		return 0;
	}

	echo '<pre>';
	printf("%50s%20s%20s\n", 'student', 'password', 'result');
	printf("------------------------------------------------------------------------------------------\n");
	for ($i=0; $i<$n; $i++){
		if ($valid[$i] == 'Y')
			printf("<font color=green>%50s%20s%20s</font>\n", $students[$i], $pass[$i], 'successfully');
		else
			printf("<font color=red>%50s%20s%20s</font>\n", $students[$i], $pass[$i], 'failed');
	}
	echo '</pre>';

	return 1;
}
?>
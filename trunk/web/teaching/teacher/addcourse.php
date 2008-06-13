<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'teacher'){
		header('Location: login.php');
		exit;
	}
?>
<?php
	if (isset($_POST['submit'])){
		include('../common/tcpclient.php');
		$d = "\001";
		$cid = addCourse($_SESSION['user_id'].$d.$_POST['description']);
		echo "course ID : $cid<br>";
		if (!$cid)
			echo 'add course failed!<br>';
		else
			addStudentsToCourse($cid, $_POST['students']);
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
	<b>Arrange a course</b><br>
	<hr><br>
	<form name="form" action="addcourse.php" method=post>
		Teacher User ID: <?php echo $_SESSION['user_id']; ?><br>
		Description: <input name='description' size=80/><br>
		Students' User ID:<br><textarea name='students' cols=60 rows=10>One line a student's user_id, like:
200431500218
200431500219
		</textarea>
		<div id="stable" style="display:none"></div>
		<br>
		<input type="button" name="preview" value="Preview" onclick="show(this.form)">
		<input type="submit" name="submit" value="Submit">
		<input type="reset" value="Reset">
	</form>
	</div>
</body>
<script language="javascript">
function show(form)
{
	if (form.preview.value == "Preview"){
		var rows=form.students.value.split("\r\n");
		form.students.style.display = "none";
		var tb = "<table border=1><th colspan=5>学号</th>";
		for (i=0; i<rows.length; i++)
		if (rows[i].length > 0)
		{
			if (i%5==0) tb+='<tr>';
			tb += '<td>'+rows[i]+'</td>';
			if (i%5==4) tb+='</tr>';
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
</html>



<?php
function addCourse($message)
{
//	return true;

	$header = sprintf("%s%08d", 'aa', strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $cid);
	$tc->close();
	return $cid;
}

function addStudentsToCourse($cid, $sts)
{
	$ss = explode("\r\n", $sts);
	$num = 0;
	$students = array();
	forEach($ss as $s){
		$s = trim($s);
		if (!empty($s)){
			$students[] = $s;
			$num ++;
		}
	}
	$d = "\001";
	$message = $cid;
	for($i=0; $i<$num; $i++){
		if (!ctype_digit($students[$i])){
			echo 'student id ['.$students[$i].'] error!<br>';
			return 0;
		}
		else
			$message .= $d.$students[$i];
	}

	$header = sprintf("%s%08d", 'eo', strlen($message));
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
	echo '<pre>';
	for ($i=0; $i<$num; $i++)
		if ($valid[$i] == 'Y')
			printf("%20s%15s\n", $students[$i], 'successfully');
		else
			printf("%20s%15s\n", $students[$i], 'failed');
	echo '</pre>';
}
?>
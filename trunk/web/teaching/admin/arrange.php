<?php
	if (isset($_GET['cid']))
		$cid = $_GET['cid'];
	else
		$cid = -1;

	if (isset($_POST['submit'])){

		include('../common/tcpclient.php');
		$d = "\001";
		$jid = addTask($_POST['description']
			    .$d.$cid
			    .$d.$_POST['year']
			    .$d.$_POST['term']);
		if ($jid){
			if (addProblems('M', $jid, '0', $_POST['rp']))
				echo 'Add Required Problems successfully!<br>';
			else
				echo 'Add Required Problems failed!<br>';

			if (addProblems('S', $jid, $_POST['shoulddo'], $_POST['op']))
				echo 'Add Optional Problems successfully!<br>';
			else
				echo 'Add Optional Problems failed!<br>';

		}
		else{
			echo 'add task failed!<br>';
		}
		echo '<a href="javascript:history.back()">Back</a>&nbsp;&nbsp;';
		echo '<a href="courses.php">Course List</a>';
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
	<b>Arrange a task</b><br>
	<hr><br>
	<form name="form" action="arrange.php?cid=<?php echo $cid; ?>" method=post>
		Course ID: <?php echo $cid; ?><br>
		Description: <input name="description" size=100/><br>
		Year: <input name='year' value="2008"/><br>
		Term: <select name="term">
			  <option value="S" selected>spring</option>
			  <option value="X">fall</option>
			  </select><br>
		<br>Required Problems:<br>
		<textarea name="rp" cols=60 rows=10>Input required problems' ID, separate by ",",           like: 1001,1002,1003</textarea><br>
		<br>Optional Problems:<br>
		How many problem should to do: <input name="shoulddo" size=10><br>
		<textarea name="op" cols=60 rows=10>Input required problems' ID, separate by ",",           like: 1001,1002,1003</textarea><br>
		<input type="submit" name="submit" value="Submit">
		<input type="reset" value="Reset">
	</form>
	</div>
</body>
</html>

<?php
function addTask($message)
{
	$header = sprintf("%s%08d", 'aj', strlen($message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $jid);
	$tc->close();
	return $jid;
}

function addProblems($type, $jid, $sd, $ps)
{
	$ss = explode(",", $ps);
	$num = 0;
	$problems = array();
	forEach($ss as $s){
		$s = trim($s);
		if (!empty($s)){
			$problems[] = $s;
			$num ++;
		}
	}
	$d = "\001";
	$message = $type.$d.$jid.$d.$sd;

	for($i=0; $i<$num; $i++){
		if (!ctype_digit($problems[$i])){
			echo 'student id ['.$problems[$i].'] error!<br>';
			return 0;
		}
		else
			$message .= $d.$problems[$i];
	}
	echo $message;

	$header = sprintf("%s%08d", 'pj', strlen($message));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($message)or die("send message failed");
	$recv = $tc->recvstr(1);
	$tc->close();
	if ($recv == 'Y')
		return true;
	else
		return false;
}
?>


<?php
	if (isset($_POST['submit'])){
		if (addTask()){
			echo '<meta http-equiv=refreah content="0;url=tasks.php">';
			exit;
		}
		else{
			echo 'add task failed!<br>';
			echo '<a href="javascript:history.back()">Back</a>';
			exit;
		}
	}
?>

<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Programming Management</title>
  <link href="t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Arrange a task</b><br>
	<hr><br>
	<form name="form" action="arrange.php" method=post>
		Class: <input name='class'/><br>
		Description: <input name="description" size=100/><br>
		Year: <input name='year' value="2008"/><br>
		Term: <select name="term">
			  <option value="S" selected>上学期</option>
			  <option value="X">下学期</option>
			  </select><br>
		Required Problems:<br>
		<textarea name="rp" cols=60 rows=10>Input required problems' ID, separate by ",",           like: 1001,1002,1003</textarea><br>
		Optional Problems:<br>
		<textarea name="op" cols=60 rows=10>Input required problems' ID, separate by ",",           like: 1001,1002,1003</textarea><br>
		<input type="submit" name="submit" value="Submit">
		<input type="reset" value="Reset">
	</form>
	</div>
</body>
</html>

<?php
function addTask()
{
	return 0;
}
?>

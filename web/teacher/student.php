<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Programming Management</title>
  <link href="t.css" rel="stylesheet" type="text/css" />
</head>
<body>
	<div id="main">
	<b>Student:<?php echo $_GET['sid']?></b><br><br>
	<table border=1 cols=2>
		<th width="30%"></th><th width="70%"></th>
		<tr><td>Name:</td><td><?php echo $_GET['name']?></td></tr>
		<tr><td>Class:</td><td><?php echo $_GET['cls']?></td></tr>
		<tr><td>Grade:</td><td><?php echo $_GET['grd']?></td></tr>
		<tr><td>Problems Solved:</td><td>1000 1001 1002 1003 1004 1005 1006 1007 1008 1009 1010 1011 1012 1013 1014 1015 1016</td></tr>
		<tr><td>Problems to be Solved:</td><td>1100 1101 1102</td></tr>
	</table>
	<br>
	<a href="javascript:history.back()">Back</a>
	</div>
	</div>
</body>
</html>
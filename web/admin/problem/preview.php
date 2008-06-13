<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}

	include('../common/format_output.php');
	$fo = new format_output();
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
    Home&nbsp;|&nbsp;
    Problems&nbsp;|&nbsp;
    Contests&nbsp;|&nbsp;
    Judge&nbsp;|&nbsp;
    User&nbsp;|&nbsp;
	Discuss&nbsp;|&nbsp;
    Logout
  </div>

  <div id="tt">
    Preview problem
  </div>


  <div class="ifm">
    <strong>Time Limit</strong>:<?php echo $_POST['time_limit']; ?>MS&nbsp;
	<strong>Memory Limit</strong>:<?php echo $_POST['memory_limit']; ?>KB<br>
    <strong>Total Submit</strong>:0&nbsp;
	<strong>Accepted</strong>:0<br>
  </div>

  <div id="main">
    <div class="ptt">Description</div>
    <div class="ptx"><?php echo $fo->formatHtml($_POST['description']); ?></div>

    <div class="ptt">Input</div>
    <div class="ptx"><?php echo $fo->formatHtml($_POST['input']); ?></div>

    <div class="ptt">Output</div>
    <div class="ptx"><?php echo $fo->formatHtml($_POST['output']); ?></div>

    <div class="ptt">Sample Input</div>
    <div class="code"><?php echo $fo->formatHtml($_POST['sample_input']); ?></div>

    <div class="ptt">Sample Output</div>
    <div class="code"><?php echo $fo->formatHtml($_POST['sample_output']); ?></div>

    <div class="ptt">Hint</div>
    <div class="ptx"><?php echo $fo->formatHtml($_POST['hint']); ?></div>

    <div class="ptt">Source</div>

    <div class="ptx">
		<div><?php echo $fo->formatHtml($_POST['source']); ?></div>
    </div>
    <br />
    <div>
      <span class="bt">Submit</span>&nbsp;&nbsp;
	  <span class="bt">Discuss</span>&nbsp;&nbsp;
      <span class="bt">Status</span>
    </div>
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

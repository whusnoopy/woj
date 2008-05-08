<?php
     $hasNewMail = check_new_mail(isset($_SESSION['user_id'])?$_SESSION['user_id']:'');
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>HomePage of Wuhan Univ. Online Judge</title>
  <link href="../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
	<div id="bar">
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
	<a href="../contest/vcontestList.php">Virtual Contests</a>&nbsp;|&nbsp;
    <a href="../submit/submit.php">Submit</a>&nbsp;|&nbsp;
    <a href="../status/status.php">Status</a>&nbsp;|&nbsp;
    <a href="../ranklist/ranklist.php">Ranklist</a>&nbsp;|&nbsp;
    <a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../user/user.php">User</a>&nbsp;|&nbsp;
    <a href="../mail/mailList.php">
      <?php
        if ($hasNewMail==true)
          echo "<font color=\"red\">Mail</font></a>";
        else
          echo "Mail</a>";
	  ?>
     &nbsp;|&nbsp;
    <a href="../faq.html" target="_blank">FAQ</a>
	</div>


<?php

function check_new_mail($user_id)
{
	if($user_id)
	    return true;
	else
		return false;
}

?>
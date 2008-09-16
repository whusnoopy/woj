<?php
     $NewMail = isset($_SESSION['mail_number'])?$_SESSION['mail_number']:0;
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <link href="/flood/style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<center>
  <div id="bar">
    <a href="/flood/index.php">Home</a>&nbsp;|&nbsp;
    <a href="/flood/problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="/flood/contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="/flood/contest/vcontestList.php">Virtual Contests</a>&nbsp;|&nbsp;
    <a href="/flood/submit/submit.php">Submit</a>&nbsp;|&nbsp;
    <a href="/flood/status/status.php">Status</a>&nbsp;|&nbsp;
    <a href="/flood/ranklist/ranklist.php">Ranklist</a>&nbsp;|&nbsp;
    <a href="/flood/discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="/flood/user/user.php">User</a>&nbsp;|&nbsp;
    <a href="/flood/mail/mailList.php">
<?php
        if ($NewMail > 0)
          echo "<font color=\"red\">Mail[$NewMail]</font>";
        else
          echo "Mail";
?>
    </a>&nbsp;|&nbsp;
    <a href="/flood/faq.html" target="_blank">FAQ</a>
  </div>

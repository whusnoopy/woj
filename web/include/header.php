<?php
  session_start();
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood, the Online Judge v3 of Wuhan Univ.</title>
  <link href="/flood/style/flood.css" rel="stylesheet" type="text/css" />
</head>
<?php
  if (isset($_SESSION['user_id']))
    $user_id = $_SESSION['user_id'];
  else
    $user_id = '';

  $hasNewMail = check_new_mail($user_id);
?>
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
<?php if ($hasNewMail==true) ?>
    <font color="red">Mail</font>";
<?php else ?>
    "Mail";</a>&nbsp;|&nbsp;
    <a href="/flood/faq.html" target="_blank">FAQ</a>
  </div>
<?php
function check_new_mail($user_id) {
  if($user_id)
    return true;
  else
    return false;
}
?>

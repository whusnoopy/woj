<?php
	session_start();
	$NewMail = isset($_SESSION['mail_number'])?$_SESSION['mail_number']:0;
?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>HomePage of Wuhan Univ. Online Judge</title>
  <link href="./style/noah.css" rel="stylesheet" type="text/css" />
</head>
<?php
    if (isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else
		$user_id = '';

?>
<body>
<center>
	<div id="bar">
    <a href="index.php">Home</a>&nbsp;|&nbsp;
    <a href="problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="contest/contestList.php">Contests</a>&nbsp;|&nbsp;
	<a href="contest/vcontestList.php">Virtual Contests</a>&nbsp;|&nbsp;
    <a href="submit/submit.php">Submit</a>&nbsp;|&nbsp;
    <a href="status/status.php">Status</a>&nbsp;|&nbsp;
    <a href="ranklist/ranklist.php">Ranklist</a>&nbsp;|&nbsp;
    <a href="discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="user/user.php">User</a>&nbsp;|&nbsp;
    <a href="mail/mailList.php">
      <?php
        if ($NewMail>0)
          echo "<font color=\"red\">Mail[$NewMail]</font></a>";
        else
          echo "Mail</a>";
	  ?>
     &nbsp;|&nbsp;
    <a href="faq.html" target="_blank">FAQ</a>
	</div>

  <div id="tt">
    Welcome to Flood
  </div>
<?php
	include('include/notice.php');
?>
  <div id="main" align="left">
	<div id="right">
		<div class="hpt">Upcoming Contest</div>
		<div class="hpb">
		<a href="http://acm.whu.edu.cn/noah/contest/contest.php?contest_id=1015"><strong>2007 WarmUp Contest 2</strong></a><br />
		Start at 2007-03-17 12:00:00
		</div>

		<div class="hpt">The Most Diligent Programmer</div>
		<div class="hpb">

		</div>


		<div class="hpt">Downloads</div>
		<div class="hpb">
		<a href="ftp://202.114.121.11/" target="_blank"><img src="image/download.jpg" alt="Downloads" align="right"  /></a>
		<div class="ir"><a href="ftp://202.114.121.11/Editor/gvim70.exe" target="_blank">gVIM 7.0</a></div>
		<div class="ir"><a href="ftp://202.114.121.11/IDE/DevC++4.9.9.2.exe" target="_blank">DEV C++ 4.9.9.2</a></div>
		<div class="ir"><a href="ftp://202.114.121.11/IDE/eclipse.rar" target="_blank">Eclipse 3.1 with CDT for Win</a></div>
		<div class="ir"><a href="ftp://202.114.121.11/IDE/MinGW-3.0.0-1.exe" target="_blank">MinGW-3.0.0.1</a></div>
		<div class="ir"><a href="ftp://202.114.121.11/Software/jdk-1_5_0_10-windows-i586-p.exe" target="_blank">JDK 1.5.0.10</a></div>
		</div>


		<div class="hpt">Links</a></div>
		<div class="hpb">
		<div class="ir"><a href="http://bbs.whu.edu.cn/bbsdoc.php?board=ACM_ICPC" target=_blank>ACM/ICPC Board of BBS in Wuhan Univ.</a></div>
		<div class="ir"><a href="http://acm.pku.edu.cn/JudgeOnline/" target=_blank>OnlineJudge of PeKing Univ.</a></div>
		<div class="ir"><a href="http://acm.zju.edu.cn/" target=_blank>OnlineJudge of ZheJiang Univ.</a></div>
		<div class="ir"><a href="http://acm.scu.edu.cn/" target=_blank>OnlineJudge of SiChuan Univ.</a></div>
		<div class="ir"><a href="http://acm.tju.edu.cn/toj" target=_blank>OnlineJudge of TianJin Univ.</a></div>
		</div>
	</div>

	<div id="left">
		<div class="hpt">Guide for Beginners</div>
		<div class="hpb">
		&nbsp;&nbsp;&nbsp;Choose a problem from <a href="problem/problemSet.php">problem sets</a>,
		then solve it, <a href="submit/submit.jsp">submit</a> your code if you are sure that your
		code is right. Sometimes there is hints after a problem, it will be useful.<br />
		<a href="faq.html"><img src="image/faq2.jpg" alt="Frequently Asked Questions" align="right"  /></a>
    &nbsp;&nbsp;&nbsp;<a href="contest/contestList.php">Contests</a> will be hold with a fixed schedule, you can take part in anyone to show yourself.<br />
    &nbsp;&nbsp;&nbsp;Read <a href="faq.html">FAQ</a> carefully first when you have any problem on how to use this system,
    many questions are already solved.<br />
    </div>

		<div class="hpt">News</div>
		<div class="hpb">
    <div class="news"><span class="newst">Mar 4<sup>th</sup>, 2007:</span> Special Judge and Restricted Function check normally work now.</div>
    <div class="news"><span class="newst">Mar 2<sup>nd</sup>, 2007:</span> Change noah to a new style.</div>
    <div class="news"><span class="newst">Feb 11<sup>th</sup>, 2007:</span> Most pages of Noah be updated to seek for a new visual effect.</div>
    <div class="news"><span class="newst">Jan 30<sup>th</sup>, 2007:</span> The most diligent programmer can be update now.</div>
    <div class="news"><span class="newst">Jan 5<sup>th</sup>, 2007:</span> Noah v2.0 project started, <a href="mailto:acm@whu.edu.cn?Subject=Suggestion for the Noah v2.0">contact us</a> for any suggestion.</div>
    <div class="news"><span class="newst">Jan 5<sup>th</sup>, 2007:</span> HomePage of Noah updated</div>

    <div class="news"><span class="newst">Mar 22<sup>nd</sup>, 2006:</span> WHU Online Judge Version 1.0 released</div>
    </div>
	</div>
</div>

<?php
	include("include/tailer.php");
?>

<?php
function check_new_mail($user_id)
{
	if($user_id)
	    return true;
	else
		return false;
}
?>
<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
	$do = true;
	if (isset($_POST['submit'])){
		if (!set_news($_POST['news']))
			$do = false;
	}
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
    <a href="../index.php">Home</a>&nbsp;|&nbsp;
    <a href="../problem/problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>

  <div id="tt">
    News List
  </div>
  <?php
    if (!$do){
		echo '<font color=red>Add news failed</font><br><br>';
    }
  ?>

  <div id="main">
  <table ><tbody>
  <tr>
    <th width="50">ID</th>
	<th width="600">Title</th>
	<th width="150">Time</th>
    <th width="100">Edit</th>
  </tr>
  <?php
	$news = array();
	get_news($news);
	$cnt = count($news);
	for ($i=0; $i<$cnt; $i+=3){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo '<td>'.$news[$i].'</td><td>'.$news[$i+1].'</td><td>'.$news[$i+2].'</td>';
		echo '<td><a href="udnews.php?nid='.$news[$i].'&news='.base64_encode($news[$i+1]).'">Edit</td>';

		echo '</tr>';
	}

  ?>
  </tbody></table>
  <br><br><br>
  <form action="setNews.php" method=post>
	Add News:<input name="news" size=120 maxlength=200/><br><br>
   <input type="submit", value="Submit" name="submit"/>&nbsp;&nbsp;<input type="reset" value="Reset">
  </form>

  </div>
  <br />



  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>

<?php
function get_news(&$news)
{

	$header = "nl00000000";

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$news = explode("\001", $recv);
	}
	else
		$news = null;
	$tc->close();
}

function set_news($news)
{
	if (empty($news)) return;
	$news = $news."\001"." ";
	$header = sprintf("%s%08d", "an", strlen($news));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($news)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

?>
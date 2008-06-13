<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location:login.php?errorMsg=please login first!');
		exit;
	}
	include('../common/tcpclient.php');
	if(isset($_GET['user_id'])){
		$user_id = $_GET['user_id'];
	}

	$user = array();
	get_user_info($user_id, $user);
	if(empty($user)){
		header("Location: error.php?errorMsg='The user doesn't exist!'");
		exit;
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
    <a href="problemList.php">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.php">Contests</a>&nbsp;|&nbsp;
    <a href="../judge/judge.php">Judge</a>&nbsp;|&nbsp;
    <a href="../user/userList.php">User</a>&nbsp;|&nbsp;
	<a href="../discuss/discussList.php">Discuss</a>&nbsp;|&nbsp;
    <a href="../logout.do.php">Logout</a>
  </div>


  <div id="tt">User Details</div>


  <div id="main">
    <table><tbody align=left>
     <tr>
      <th width=100></th>
      <th colSpan=2><?php echo $user_id; ?></th>
      <th width=100></th>
     </tr>
     <tr class=tro>
      <td></td>
      <td width=125><strong>Rank:</strong></td>
      <td width=625><?php echo $user[0]; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>Solved:</strong></td>
      <td><?php echo $user[1]; ?></td>
      <td></td>
     </tr>
     <tr class=tro>
      <td></td>
      <td><strong>Submissions:</strong></td>
      <td><?php echo $user[2]; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>School:</strong></td>
      <td><?php echo $user[3]; ?></td>
      <td></td>
     </tr>
     <tr class=tro>
      <td></td>
      <td><strong>Email:</strong></td>
      <td><?php echo $user[4]; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>Solved Problem:</strong></td>
      <td>
	  <?php
	      for($i=5; $i<count($user); $i++){
			$pid = $user[$i];
			echo "<a href=../problem/problem.php?problem_id=$pid>$pid &nbsp;</a>";
		  }
      ?>
	  </td>
      <td></td>
     </tr>
    </tbody></table>
  <br />
  <div>
    <span class="bt"><a href="../ranklist/ranklist.php">Back To Ranklist</a></span>
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

<?php
function get_user_info($user_id, &$user)
{

	///////////////////////////////
/*	$d = "\001";
	$recv = "1".$d."100".$d."120".$d."whu".$d."mcje2004@126.com".$d."1001".$d."1002".$d."1003".$d."1004";
	$user = explode("\001", $recv);
	return;
*/	////////////////////////////////

	if(empty($user_id)){
		$user = null;
		return;
	}

	$header = sprintf("%s%08d", "ui", strlen($user_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($user_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$user = explode("\001", $recv);
	}
	else $user = null;
	$tc->close();
	return ;
}
?>
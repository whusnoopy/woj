<?php
	include('../include/header.php');
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

  <div id="tt">
    User Status - <a href="../mail/sendMail.php?to_user=<?php echo $user_id; ?>"><?php echo $user_id; ?></a>
  </div>

 <?php
	include('../include/notice.php');
 ?>

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

<?php
	include('../include/tailer.php');
?>

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
<?php
	include('../include/header.php');
	include('../common/tcpclient.php');
	include('classes/mail_list_t.php');

?>

<?php
	if(!$user_id){
		header("Location: ../user/login.php?origURL=../mail/mailList.php");
		exit;
	}

	if(isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';


?>

  <div id="tt"><?php echo $user_id; ?>'s mailbox</div>

<?php
	include('../include/notice.php');
?>
  <div id="main">
    <table><tbody>
  <tr>
    <th width=80>No.</td>
    <th width=170>From</td>
    <th width=500>Title</td>
    <th width=200>Date</th>
  </tr>

<?php

	$ml = new mail_list_t($user_id, $start);
	$ml->getResult();
	$rows = $ml->getRow();

	for ($i=0; $i<$rows; $i++){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo '<td>'.$ml->getMail_id($i).'</td>';
		$from = $ml->getFrom_user($i);
		echo "<td><a href=\"../status/userStatus.php?user_id=${from}\">${from}</a></td>";
		echo '<td><a href="mail.php?mail_id='.$ml->getMail_id($i).'">'.$ml->getTitle($i).'</a></td>';
		echo '<td>'.$ml->getIn_date($i).'</td>';
	}

?>
    </tbody></table>
    <br />
<?php
    if ($start > 0){
		$pre = $start - 1;
		echo "<span class=bt><a href=\"mailList.php?start=$pre\">Previous</a></span>&nbsp;&nbsp;";
	}

	echo '<span class=bt><a href="sendMail.php">Send mail</a></span>&nbsp;&nbsp;';

    if ($rows == 20){
		$next = $start+1;
		echo "<span class=bt><a href=\"mailList.php?start=$next\">Next</a></span>&nbsp;&nbsp;";
	}

?>
<br />
  </div>

<?php
	include('../include/tailer.php');
?>

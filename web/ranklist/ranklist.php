<?php
  include('../include/header.php');
  echo '<div id=tt>User Ranklist</div>';
  include('../include/notice.php');
  include('../common/tcpclient.php');
  include('classes/ranklist_t.php');
?>

<?php
    if(isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if(isset($_GET['of1']))
		$of1 = $_GET['of1'];

	if($of1 == 'submit')
	    $of1 = '1';
	else if($of1 == 'ratio')
		$of1 = '2';
	else
	    $of1 = '0';

	$rk = new ranklist_t($start, $of1);
	$rk->getResult();
	$rows = 20;
?>

<div id="main">

<table><tbody>
  <tr>
    <th width="80">No.</th>
    <th width="120">User ID</th>
    <th width="450">Nick Name</th>
    <th width="100"><a href="ranklist.php">Solved</a></th>
    <th width="100"><a href="ranklist.php?of1=submit">Submit</a></th>
    <th width="100"><a href="ranklist.php?of1=ratio">Ratio</a></th>
  </tr>

 <?php

	 for ($i=0, $rank = $start * $rows + 1; $i<$rk->getRow(); $i++, $rank++){
		 if($i%2==0)
			 echo '<tr class=tro>';
		 else
			 echo '<tr class=tre>';
		 echo "<td height=26>$rank</td>";
	     $user_id = $rk->getUser_id($i);
		 $nick = $rk->getNick($i);
		 $solved = $rk->getSolveds($i);
		 $submits = $rk->getSubmits($i);
		 $ratio = sprintf("%.2f%s",$solved * 100 / $submits, '%');
		 echo "<td><a href=../status/userStatus.php?user_id=$user_id>$user_id</a></td>";
         echo "<td>$nick</td>";
		 echo "<td>$solved</td>";
		 echo "<td>$submits</td>";
		 echo "<td>$ratio</td>";
	 }
?>
</tbody></table>
<br>
<div>
<?php

	if($start > 0){
		echo '<span class=bt>';
		$pre = $start-1;
		if(empty($_GET['of1']))
			echo "<a href=ranklist.php?start=$pre>";
		else
			echo "<a href=ranklist.php?start=$pre&of1=$_GET[of1]>";
	    echo '&nbsp;Prev Page&nbsp;</a></span>';
	}
	echo '&nbsp;';
	echo '<span class=bt>';
	$next = $start+1;
	if(empty($_GET['of1']))
		echo "<a href=ranklist.php?start=$next>";
	else
		echo "<a href=ranklist.php?start=$next&of1=$_GET[of1]>";
	echo '&nbsp;Next Page&nbsp;</a></span>';
?>
 </div><br>
 </div>

<?php
	include('../include/tailer.php');
?>


<?php
	session_start();
	include('../common/tcpclient.php');
	include('classes/contest_list_t.php');

	if(isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';

	$cl = new contest_list_t($start, 'C');
	$cl->getResult();
	$rows = $cl->getRow();
?>
<?php
	include('../include/header.php');
    echo '<div id=tt>Contests List</div>';
	include('../include/notice.php');
?>
<div id="main">
<table ><tbody>
  <tr>
    <th width="50">ID</th>
	<th width="510">Title</th>
	<th width="180">Start</th>
    <th width="150">Status</th>
    <th width="80">Type</th>
  </tr>

<?php

	for($i=0; $i<$rows; $i++){
		if($i%2==0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		$contestID = $cl->getID($i);
		echo "<td>$contestID</td>";
		echo "<td><a href=contest.php?contest_id=$contestID>".$cl->getTitle($i)."</a></td>";
		echo "<td>".$cl->getStart_time($i)."</td>";
		echo "<td>".$cl->getState($i)."</td>";
		echo "<td>".$cl->get_type($i)."</td>";
		echo "</tr>";
	}
?>
</tbody></table>

<br />

<?php
  $current = intval($start);
  if ( $current > 0){
	  $pre = $current - 1;
      echo "<span class=bt><a href=\"contestStanding.php?start=$pre&contest_id=$contest_id&title=$title\">
    &nbsp;Prev Page&nbsp;</a></span>&nbsp;";
  }
  if ($rows == 20){
	  $next = $current + 1;
      echo "<span class=bt><a href=\"contestStanding.php?start=$next&contest_id=$contest_id&title=$title\">&nbsp;Next Page&nbsp;</a></span>";
  }
?>
  </div><br>


<?php
	include('../include/tailer.php');
?>
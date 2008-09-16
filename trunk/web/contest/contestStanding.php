<?php
	include('../common/tcpclient.php');
	include('classes/contest_standing_t.php');

	if(isset($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else{
	//	echo 'contest does not exit';
	//	exit;
	}
	if (isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	$title = $_GET['title'];

	$cs = new contest_standing_t($contest_id, $start);
	$cs->getResult();
?>

<?php
	include('../include/header.php');
	echo '<title>Contest Standing</title>';
	echo "<div id=tt>Contests - $title</div>";
	include('../include/notice.php');
?>
  <div id="main">

<table width = "950"><tbody>
  <tr>
    <th width="45">Rank</th>
    <th width="150">Nick Name</th>
    <th width="30">AC</th>
    <th width="75">Penalty</th>
<?php

	$problem_num = $cs->getProblem_num();
	$rows = $cs->getRow();
	$character = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	for($i=0; $i<$problem_num; $i++)
		echo '<th>'.$character{$i}.'</th>';
	echo '</tr>';

	for ($i=0,$rank=$start*20+1; $i<$rows; $i++, $rank++){
		if ($i%2==0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo "<td>$rank</td>";
		echo '<td><a href="../status/userStatus.php?user_id='.($cs->getUser_id($i)).'">';
    if (strlen($cs->getNick($i)) == 0)
      echo $cs->getUser_id($i);
    else
      echo $cs->getNick($i);
    echo '</a></td>';

// TODO: All penalty and a certain problem pennalty needs to check with server
// module, there are confused result here
		echo '<td>'.$cs->getAC($i).'</td>';
		echo '<td>'.$cs->getPenalty($i).'</td>';
		for($j=0; $j<$problem_num; $j++) {
      echo '<td>';
      $submit = $cs->getProblem_submit($i, $j);
      if (($penalty = $cs->getProblem_Penalty($i, $j)) != '00:00:00') {
        echo "$penalty<br>";
        $submit = $submit + 1;
      }
			if ($submit != '0')
				echo $submit;
      echo '</td>';
    }
	}

?>
</tbody></table>

  <br><div>

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
  </div>

<?php
    include('../include/tailer.php');
?>

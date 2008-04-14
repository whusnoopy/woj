<?php
	include('../include/header.php');
	include('classes/problem_list_t.php');


	if($_POST['field'] == 'title'){
		$title = $_POST['key'];
		$source = '?';
	}
	else{
		$title = '?';
		$source = $_POST['key'];
	}

	if(isset($_SESSION['user_id']))
		$user_id = $_SESSION['user_id'];
	else
		$user_id = '?';
?>

  <div id="tt">
    Problem Search Result - <?php echo $_POST['field'].' : '.$_POST['key'];?>
  </div>
<?php
	include ('../include/notice.php');
?>

  <div id="main">

    <table><tbody>
    <tr>
      <th width="50">&nbsp;</th>
      <th width="80">ID</a></th>
      <th width="300">Title</th>
      <th width="370">Source</th>
      <th width="150">Ratio&nbsp;(AC/Total)</th>
    </tr>

<?php

	$pl = new problem_list_t('?', $user_id, $title, $source);
	$pl->getResult();
	$rows = $pl->getRow();

	for ($i=0; $i<$rows; $i++){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo '<td>';
		$type = $pl->getResult_type($i);
		if ($user != '?' && $type>'0'){
			if ($type == '1')
				echo '<img src="../image/accepted.gif" />';
			else
				echo '<img src="../image/wrong.gif" />';
		}
		echo '</td>';
		$pid = $pl->getProblem_id($i);
		echo "<td>$pid</td>";
		echo "<td><a href=\"problem.php?problem_id=$pid\">".$pl->getTitle($i).'</a></td>';
		echo '<td>'.$pl->getSource($i).'</td>';
		$ac = $pl->getAC($i);
		$total = $pl->getTotal($i);
		if (empty($total))
			$ratio = '0.00';
		else
			$ratio = sprintf("%.2f", $ac*100/$total);
		echo "<td>$ratio%($ac/$total)</td>";
	}
?>

</tbody></table>

  <br><br>
  <span class="bt"><a href="problemList.php">Back to Problem List</a></span>
  <br><br>

  </div>


<?php
    include ('../include/tailer.php');
?>

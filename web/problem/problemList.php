<?php
	include('../common/tcpclient.php');
	include('classes/problem_list_t.php');
	$pl = new problem_list_t($start, $user_id, '?', '?');
	$pl->getResult();
	include('../include/header.php');


	if (isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (empty($user_id))
		$user_id = '?';


	echo "<div id=tt>Problems Volume $start</div>";
	include('../include/notice.php');
?>

  <div id="main">

  <table><tbody align=center>
    <tr>
      <th width="80">&nbsp;</th>
      <th width="100"><a href="problemList.php?type=0&start=<?php echo $start; ?>">ID</a></th>
      <th width="590">Title</th>
      <th width="180"><a href="problemList.php?type=0&start=<?php echo $start; ?>">Ratio</a>&nbsp;(<a href="problemList.php?type=2&start=<?php echo $start; ?>">AC</a>/<a href="problemList.php?type=3&start=<?php echo $start; ?>">Total</a>)</th>
    </tr>
<?php
	$pages = $pl->getPages();
	$rows = $pl->getRow();
	for ($i=0; $i<$pages; $i++)
		echo "<a href=\"problemList.php?start=$i\"><b>[$i] </b></a>";
	echo '<br>';
	for ($i=0; $i<$rows; $i++){
		if ($i%2 == 0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		echo '<td>';
		$type = $pl->getResult_type($i);
		if ($type == '1')
			echo '<img src="../image/accepted.gif" />';
		else if ($type == '2')
			echo '<img src="../image/wrong.gif" />';

		echo '</td>';
		$pid = $pl->getProblem_id($i);
		echo "<td>$pid</td>";
		echo "<td><a href=\"problem.php?problem_id=$pid\">".$pl->getTitle($i).'</a></td>';
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
  <br>
  <br />
  <div>
    <form method=post action="searchList.php?start=0" >
	<strong>Search:</strong>&nbsp;<input name=key type=text value='' size=50 maxlength="255" />&nbsp;
    <strong>By:</strong>&nbsp;<select name=field><option value="title">Title</option><option value="source">Source</option></select>&nbsp;
    <input type=submit value=GO />
    </form>
  </div>
  <br>
  </div>

<?php
	include('../include/tailer.php');
?>

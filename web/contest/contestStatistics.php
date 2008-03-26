
<?php
	include('../include/header.php');
    echo "<div id=tt>Contest Statistics - $_GET[title]</div>";
	include('../include/notice.php');
	include('classes/contest_statistics_t.php');

	if(isset($_GET['contest_id']))
		$contest_id = $_GET['contest_id'];
	else{
//		echo 'contest not exist';
//		exit;
	}

?>
  <div id="main">

<table><tbody>
  <tr>
    <th width="50"></th>
    <th width="75">AC</th>
    <th width="75">PE</th>
    <th width="75">CE</th>
    <th width="75">WA</th>
    <th width="75">TLE</th>
    <th width="75">RE</th>
    <th width="75">MLE</th>
    <th width="75">OLE</th>
    <th width="75">Total</th>
    <th width="75">C/C++</th>
    <th width="75">Java</th>
    <th width="75">Pascal</th>
  </tr>

<?php
	$cs = new contest_statistics_t($contest_id);
	$cs->getResult();
	$rows = $cs->getRow();

	$character = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$total = array();
	for ($i=0; $i<$rows; $i++){
		if ($i%2==0)
			echo '<tr class=tro>';
		else
			echo '<tr class=tre>';
		$pid = $cs->getProblem_id($i);
		echo "<td><a href=\"../problem/problem.php?problem_id=$pid\">".$character{$i}."</a></td>";
		echo '<td>'.$cs->getAC($i).'</td>'; $total['AC'] += $cs->getAC($i);
		echo '<td>'.$cs->getPE($i).'</td>'; $total['PE'] += $cs->getPE($i);
		echo '<td>'.$cs->getCE($i).'</td>'; $total['CE'] += $cs->getCE($i);
		echo '<td>'.$cs->getWA($i).'</td>'; $total['WA'] += $cs->getWA($i);
		echo '<td>'.$cs->getTLE($i).'</td>'; $total['TLE'] += $cs->getTLE($i);
		echo '<td>'.$cs->getRE($i).'</td>'; $total['RE'] += $cs->getRE($i);
		echo '<td>'.$cs->getMLE($i).'</td>'; $total['MLE'] += $cs->getMLE($i);
		echo '<td>'.$cs->getOLE($i).'</td>'; $total['OLE'] += $cs->getOLE($i);
		echo '<td>'.$cs->getTotal($i).'</td>'; $total['Total'] += $cs->getTotal($i);
		echo '<td>'.$cs->getC_CPP($i).'</td>'; $total['C_CPP'] += $cs->getC_CPP($i);
		echo '<td>'.$cs->getJAVA($i).'</td>'; $total['JAVA'] += $cs->getJAVA($i);
		echo '<td>'.$cs->getPASCAL($i).'</td>'; $total['PASCAL'] += $cs->getPASCAL($i);
		ECHO '<tr>';
	}
	echo '<tr style="background-color:#ffff7f;height:24px;font-weight:bold">';
	echo '<td>Total</td>';
	echo '<td>'.$total['AC'].'</td>';
	echo '<td>'.$total['PE'].'</td>';
	echo '<td>'.$total['CE'].'</td>';
	echo '<td>'.$total['WA'].'</td>';
	echo '<td>'.$total['TLE'].'</td>';
	echo '<td>'.$total['RE'].'</td>';
	echo '<td>'.$total['MLE'].'</td>';
	echo '<td>'.$total['OLE'].'</td>';
	echo '<td>'.$total['Total'].'</td>';
	echo '<td>'.$total['C_CPP'].'</td>';
	echo '<td>'.$total['JAVA'].'</td>';
	echo '<td>'.$total['PASCAL'].'</td>';
	echo '</tr>';

?>
<tbody></table>
  </div>

<?php
	include('../include/tailer.php');
?>

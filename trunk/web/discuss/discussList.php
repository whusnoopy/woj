
<?php

	include('../include/header.php');
	echo '<div id="tt">Discuss </div>';
	include('../include/notice.php');
	include('classes/discuss_list_t.php');

	if (isset($_GET['pid']) && !empty($_GET['pid']))
		$problem_id = $_GET['problem_id'];
	else
		$problem_id = '0';
	if (isset($_GET['cid']) && !empty($_GET['cid']))
		$contest_id = $_GET['cid'];
	else
		$contest_id = '0';
	if (isset($_GET['uid']) && strlen($_GET['uid']) > 0)
		$user_id = $_GET['uid'];
	else
	    $user_id = '?';
	if (isset($_GET['start']) && !empty($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (isset($_GET['title']) && strlen($_GET['title']))
		$title = $_GET['title'];
	else
		$title = '?';

	$dl = new discuss_list_t($title, $problem_id, $contest_id, $user_id, $start);
	$dl->getResult();
	$rows = $dl->getRow();
?>

</center>
<br>
<div id="mainp">

<?php
	for ($i=0; $i<$rows; $i++){
		$level = $dl->getLevel($i);
		if($i && $level==1)
			echo '<hr width=900 size=0/>';
		for ($j=1; $j<$level; $j++)
			echo "&nbsp;&nbsp;&nbsp;&nbsp";
		if ($level==1)
			echo "+&nbsp;";
		else
			echo ">&nbsp;";
		echo '<a href="discuss.php?message_id='.($dl->getDiscuss_id($i)).'">'.($dl->getTitle($i)).'</a>&nbsp';
		echo '<strong>'.$dl->getUser_id($i).'</strong>&nbsp;';
		echo $dl->getIn_date($i).'&nbsp;';
		echo '<strong>Promble-'.$dl->getProblem_id($i).'</strong><br/>';
	}
?>

<br>

</div>
<br>
<center>
<?php
	include('../include/tailer.php');
?>
<?php
	include('../include/header.php');
	echo '<div id="tt">Discuss </div>';
	include('../include/notice.php');

	if(isset($_GET['problem_id']) && !empty($_GET['problem_id']))
		$problem_id = $GET['problem_id'];
	else
		$problem_id = '?';

?>



<?php
	include('../include/tailer.php');
?>
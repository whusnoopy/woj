<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'teacher'){
		header('Location: login.php');
		exit;
	}
	include('../common/teaching.config.php');
	$filename = '';
	ForEach ($FILETYPE as $ft)
		if (file_exists($FILEDIR.$_GET['sid'].$ft)){
			$filename = $FILEDIR.$_GET['sid'].$ft;
			break;
	}

	if($filename)
	{
		Header('Content-type: application/force-download');
		Header('Accept-Ranges: bytes');
		Header('Accept-Length: '.filesize($filename));
		Header('Content-Disposition: attachment; filename='.basename($filename));
		readfile($filename);
	}
	else
		echo 'File'.$filename.' does not exit!<br><a href="javascript:history.back()">Back</a>';

?>


<?php

	include('../common/tcpclient.php');

	$errorMsg = '';
	if (empty($_POST['problem_id']))
		$errorMsg = 'problem_id cannot be NULL';
	else if (empty($_POST['title']))
		$errorMsg = 'title cannot be NULL';
	else if (empty($_POST['description']))
		$errorMsg = 'description cannot be NULL';
	else if (empty($_POST['input']))
		$errorMsg = 'input cannot be NULL';
	else if (empty($_POST['output']))
		$errorMsg = 'output cannot be NULL';
	else if (empty($_POST['sample_input']))
		$errorMsg = 'sample_input cannot be NULL';
	else if (empty($_POST['sample_output']))
		$errorMsg = 'sample_output cannot be NULL';
	else if (empty($_POST['time_limit']))
		$errorMsg = 'time_limit cannot be NULL';
	else if (empty($_POST['memory_limit']))
		$errorMsg = 'memory_limit cannot be NULL';

	if(!empty($errorMsg)){
		echo $errorMsg;
		exit;
	}

	if (isset($_POST['spj']))
		$spj = 'Y';
	else
		$spj = 'N';
	if (isset($_POST['updatefiles']))
		$update_file_flag = 'Y';
	else
		$update_file_flag = 'N';

	$d = "\001";
	$problem_message  =   $_POST['problem_id'].$d.
						  $_POST['title'].$d.
						  $_POST['description'].$d.
						  $_POST['input'].$d.
						  $_POST['output'].$d.
						  $_POST['sample_input'].$d.
						  $_POST['sample_output'].$d.
						  (empty($_POST['hint']) ? ' ' : $_POST['hint']).$d.
						  (empty($_POST['source']) ? ' ' : $_POST['source']).$d.
						  $_POST['time_limit'].$d.
						  $_POST['case_time_limit'].$d.
						  $_POST['memory_limit'].$d.
						  $spj.$d.
						  $update_file_flag;

	if (!update_problem($problem_message)){
		echo 'failed to update problem';
		exit;
	}
	echo 'successfully to update problem<br>';

//////////////////Upload files//////////////////////////////////////////

	if ($update_file_flag == 'Y'){
		$dir = "../../files/$problem_id/";
		$time = time();

		echo 'upload files now...<br>';

		//upload pictures...
		$picture_file_names = array();
		for ($i=1; $i<=5; $i++){
			$pic = 'pic'.$i;
			if ($_FILES[$pic]['size'] > 0){
				if (move_uploaded_file($_FILES[$pic]['tmp_name'], $dir.$time."_".$_FILES[$pic]['name']))
					$picture_file_names[] = $dir.$time."_".$_FILES[$pic]['name'];
				else{
					echo 'failed uploaded'.$_FILES[$pic]['name'];
					exit;
				}
			}
		}

		//upload common files
		$common_file_names = array();
		for ($i=1; $i<=5; $i++){
			$common = 'common'.$i;
			if ($_FILES[$common]['size'] > 0){
				if (move_uploaded_file($_FILES[$common]['tmp_name'], $dir.$time."_".$_FILES[$common]['name']))
					$common_file_names[] = $dir.$time."_".$_FILES[$common]['name'];
				else{
					echo 'failed uploaded '.$_FILES[$common]['name'];
					exit;
				}
			}
		}

		//upload input/output files...
		$input_file_names = array();
		$output_file_names = array();
		$spj_file_name = '';
		if ($spj == 'N'){
			for ($i=1; $i<=10; $i++){
				$input = 'in'.$i;
				$output = 'out'.$i;
				if ($_FILES[$input]['size'] > 0 && $_FILES[$output]['size'] > 0){
					if (move_uploaded_file($_FILES[$input]['tmp_name'], $dir.$_FILES[$input]['name'])
					 && move_uploaded_file($_FILES[$output]['tmp_name'], $dir.$_FILES[$output]['name'])){
						$input_file_names[] = $dir.$_FILES[$input]['name'];
						$output_file_names[] = $dir.$_FILES[$output]['name'];
					}
					else{
						echo "failed uploaded input/output file_$i";
						exit;
					}
				}
			}
		}
		else{
			//upload special judge files...
			if ($_FILES['spj_file']['size'] > 0)
				if (move_uploaded_file($_FILES['spj_file']['tmp_name'], $dir.$_FILES['spj_file']['name'])){
					$spj_file_name = $_FILES['spj_file']['name'];
			}
			else{
				echo 'failed uploaded '.$_FILES['spj_file'];
				exit;
			}
		}


		echo 'successfully uploaded files<br>';

	//////////////////Send files///////////////////////////////////////////////


		forEach ($picture_file_names as $filename){
			if (!send_file($filename, $problem_id)){
				echo "send $filename failed!".'<br>';
				exit;
			}
		}
		echo 'sent pic files<br>';

		forEach ($common_file_names as $filename){
			if (!send_file($filename, $problem_id)){
				echo "send file: [$filename] failed!".'<br>';
				exit;
			}
		}
		echo 'send common files<br>';

		if (!send_in_out_files($input_file_names, $output_file_names, $problem_id)){
			echo 'send i/o files failed<br>';
			exit;
		}

		if ($spj == 'Y'){
			if (!send_file($spj_file_name, $problem_id)){
				echo 'send spj files: [$spj_file_name] failed<br>';
				exit;
			}
		}
		echo 'send i/o files<br>';
		echo 'successfully to update files<br>';
	}

?>

<?php

function update_problem(&$problem_message)
{
//	return '1001';

	$header = sprintf("%s%08d", "up", strlen($problem_message));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_message)or die("send message failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;

}

function send_file($filename, $problem_id)
{
//	return true;

	$header = sprintf("%s%08d", "fp", strlen(basename($filename)));
	$content = file_get_contents($filename);

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header: [$header] failed");
	$tc->sendstr(basename($filename)) or die("send filename: [$filename] failed");
	$header = sprintf("%010d", $problem_id);
	$tc->sendstr($header) or die("send problem_id: [$header] failed");
	$header = sprintf("%010d", strlen($content));
	$tc->sendstr($header) or die("send content length: [$header] failed");;
	$tc->sendstr($content) or die("send content failed");
	if ($tc->recvstr(1) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

function send_in_out_files(&$input_file_names, &$output_file_names, $problem_id)
{
//	return true;

	$cnt = min(count($input_file_names), count($output_file_names));

	$header = sprintf("%s%08d", "io", $cnt);
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$header = sprintf("%010d", $problem_id);
	$tc->sendstr($header) or die("send header failed");

	for ($i=0; $i<$cnt; $i++){
		$content = file_get_contents($input_file_names[$i]);
		$tc->sendstr(sprintf("%010d", strlen($content)));
		$tc->sendstr($content);
		$content = file_get_contents($output_file_names[$i]);
		$tc->sendstr(sprintf("%010d", strlen($content)));
		$tc->sendstr($content);
	}

	echo 'sent i/o files<br>';
	if (($tc->recvstr(1)) == 'Y'){
		$tc->close();
		return true;
	}
	$tc->close();
	return false;
}

?>
<?php

	echo 'upload files now...<br>';

	$errorMsg = '';
	if (empty($_POST['title']))
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

	if ($_FILES['pic']['size'] > 0){
		if (move_uploaded_file($_FILES['pic']['tmp_name'], './uploads/'.$_FILES['pic']['name']))
			echo 'success uploaded'.$_FILES['pic']['name'];
		else
			echo 'failed uploaded'.$_FILES['pic']['name'];
	}

	if ($spj == 'N'){
		for ($i=1; $i<=5; $i++){
			$input = 'in'.$i;
			$output = 'out'.$i;
			if ($_FILES[$input]['size'] > 0 && $_FILES[$output]['size'] > 0){
				echo $input.$output;
				if (move_uploaded_file($_FILES[$input]['tmp_name'], './uploads/'.$_FILES[$input]['name'])
				 && move_uploaded_file($_FILES[$output]['tmp_name'], './uploads/'.$_FILES[$output]['name'])){
					echo "succuss uploaded input/output file_$i";
				}
				else
					echo "failed uploaded input/output file_$i";
			}
		}
	}
	else{
		if ($_FILES['spj_file']['size'] > 0)
			move_uploaded_file($_FILES['spj_file']['tmp_name'], './uploads/'.$_FILES['spj_file']['name']);
	}

	$d = "\001";
	$problem_message  =   $_POST['title'].$d.
						  $_POST['description'].$d.
						  $_POST['input'].$d.
						  $_POST['output'].$d.
						  $_POST['sample_input'].$d.
						  $_POST['sample_output'].$d.
						  (empty($_POST['hint']) ? '?' : $_POST['hint']).$d.
						  (empty($_POST['source']) ? '?' : $_POST['source']).$d.
						  $_POST['time_limit'].$d.
						  $_POST['case_time_limit'].$d.
						  $_POST['memory'].$d.
						  $spj;
	$problem_id = add_problem(&$problem_message);
	echo 'done';

?>

<?php

function add_problem($problem_message)
{
	return '1';
}


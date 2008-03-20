
<?php
	include('..\include\header.php');
	include('classes\user_status_t.php');
	if(isset($_GET['user_id'])){
		$user_id = $_GET['user_id'];
	}
	$user_status = new user_status_t('$user_id');
	if(!$user_status->exist()){
		header("Location: error.php?errorMsg='The user doesn't exist!'");
		exit;
	}
	$solved = $user_status->get_solved();
	$submission = $user_status->get_submission();
	$rank = $user_status->get_rank();
	$solved_result = $user_status->get_solved_result();
	$school = $user_status->get_school();
	$email = $user_status->get_email();
?>

  <div id="tt">
    User Status - <a href="../mail/sendMail.php?to_user=<?php echo $user_id; ?>"><?php echo $user_id; ?></a>
  </div>

 <?php
	include('..\include\notice.php');
 ?>

  <div id="main">
    <table><tbody align=left>
     <tr>
      <th width=100></th>
      <th colSpan=2><?php echo $user_id; ?></th>
      <th width=100></th>
     </tr>
     <tr class=tro>
      <td></td>
      <td width=125><strong>Rank:</strong></td>
      <td width=625><?php echo $rank; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>Solved:</strong></td>
      <td><?php echo $solved; ?></td>
      <td></td>
     </tr>
     <tr class=tro>
      <td></td>
      <td><strong>Submissions:</strong></td>
      <td><?php echo $submissions; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>School:</strong></td>
      <td><?php echo $school; ?></td>
      <td></td>
     </tr>
     <tr class=tro>
      <td></td>
      <td><strong>Email:</strong></td>
      <td><?php echo $email; ?></td>
      <td></td>
     </tr>
     <tr class=tre>
      <td></td>
      <td><strong>Solved Problem:</strong></td>
      <td>
	  <?php
	      forEach ($solved_result as $valus){
			echo "<a href=../problem/problem.php?problem_id=$valus>$valus &nbsp;</a>";
		  }
      ?>
	  </td>
      <td></td>
     </tr>
    </tbody></table>
  <br />
  <div>
    <span class="bt"><a href="../ranklist/ranklist.php">Back To Ranklist</a></span>
  </div>
  <br />
  </div>

<?php
	include('..\include\tailer.php');
?>

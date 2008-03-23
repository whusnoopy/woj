<?php

 include('../include/header.php');
 include('classes/solution_status_t.php');
 include('classes/problem_solution_t.php');

 if(isset($_GET['start']))
	 $rank_num = $_GET['start'];
 else
	$rank_num = 0;
 if(isset($_GET['problem_id']))
	 $problem_id = $_GET['problem_id'];
 else
	$problem_id = 0;

 ///****************////
 $user_id = 'magiii';
 ///***************////

 $order = 0;
 $no_of_row = 20;
 $solution_status = new solution_status_t($problem_id); //×ó±ß
 $solution_status->initialize();
 $problem_solution = new problem_solution_t($problem_id); //ÓÒ±ß
 $problem_solution->user_id = $user_id;
 $problem_solution->start_row = $rank_num;
 $problem_solution->no_of_row = $no_of_row;

 $total_row = $problem_solution->get_total_row();
 $status = $problem_solution->get_status();
?>

<?php
/*
<c:set var="order" value="0" />

<jsp:useBean id="solutionStatBean" class="com.whu.noah.beans.status.SolutionStatBean" scope="page" />
<jsp:useBean id="problemSolutionBean" class="com.whu.noah.beans.status.ProblemSolutionBean" scope="page" />
<jsp:useBean id="statusDisplayBean" class="com.whu.noah.beans.status.StatusDisplayBean" scope="page" />

<jsp:setProperty name="solutionStatBean" property="dataSource" value="${applicationScope.appDataSource}" />
<jsp:setProperty name="problemSolutionBean" property="dataSource" value="${applicationScope.appDataSource}" />

<jsp:setProperty name="solutionStatBean" property="problem_id" value="${param.problem_id}" />
<jsp:setProperty name="problemSolutionBean" property="problem_id" value="${param.problem_id}" />

<c:set var="noOfRows" value="20" />

<jsp:setProperty name="problemSolutionBean" property="user_id" value="${validUser.uid}" />
<jsp:setProperty name="problemSolutionBean" property="startRow" value="${param.start}" />
<jsp:setProperty name="problemSolutionBean" property="noOfRow" value="${noOfRows}" />

<c:set var="totalRow" value="${problemSolutionBean.totalRow}" />
<c:set var="status" value="${problemSolutionBean.status}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />
*/
?>

  <div id="tt">
  Problem Status - <a href="../problem/problem.php?problem_id=<?php echo $problem_id; ?>" ><?php echo $problem_id; ?></a>
  </div>
<?php
	include('../include/notice.php');
?>

  <div id="mainp">
	<div id="rightp">
     <table width=730><tbody>
      <tr>
       <th width="60">Rank</th>
       <th width="70">Run ID</th>
       <th width="130">User</th>
       <th width="70">Memory</th>
       <th width="60">Time</th>
       <th width="90">Language</th>
       <th width="80">Length</th>
       <th width="170">Submit Time</th>
      </tr>
<?php
	  $line_color= 0;
	  foreach($status as $row){
		  if($line_color== 0){
			  echo '<tr class=tro>';
			  $line_color = 1;
		  }
		  else{
			  echo '<tr class=tre>';
			  $line_color = 0;
		  }
		  $rank_num++;
		  echo "<td>$rank_num</td>";
		  echo "<td>$row[solution_id]</td>";
          echo "<td><a href=../status/userStatus.php?user_id=$row[user_id]>$row[user_id]</a></td>";
          echo "<td>$row[memory]K</td>";
          echo "<td>$row[time]MS</td>";
		  echo "<td>";
		  $order = $problem_solution->order + 1;
		  $is_valid = $problem_solution->is_valid();
		  if ($is_valid)
			  echo "<a href=../source/source.php?solution_id=$row[solution_id] target=_blank>$row[language]</a>";
		  else
			  echo $row['language'];

		  echo "</td>";
          echo "<td>$row[code_length]</td>";
          echo "<td>$row[in_date]</td>";
          echo "</tr>";
	  }
?>
     </tbody></table>
    </div>
	<div id="leftp">
     <table align=left><tbody>
      <tr>
        <th width=160px>Status</th>
        <th width=60px>Count</th>
      </tr>
      <tr class=tro><td><strong>User Submitted</strong></td><td><?php $solution_status->userSubmited; ?></td></tr>
      <tr class=tre><td><strong>Users Solved</strong></td><td><?php $solution_status->userSolved; ?></td></tr>
      <tr class=tro><td><strong>Total Submits</strong></td><td><?php $solution_status->totalSubmits; ?></td></tr>
      <tr class=tre><td><strong>Accepted</strong></td><td><?php $solution_status->accepted; ?></td></tr>
      <tr class=tro><td><strong>Presentation Error</strong></td><td><?php $solution_status->pe; ?></td></tr>
      <tr class=tre><td><strong>Time Limit Exceed</strong></td><td><?php $solution_status->tle; ?></td></tr>
      <tr class=tro><td><strong>Memory Limit Exceed</strong></td><td><?php $solution_status->mle; ?></td></tr>
      <tr class=tre><td><strong>Wrong Answer</strong></td><td><?php $solution_status->wa; ?></td></tr>
      <tr class=tro><td><strong>Runtime Error</strong></td><td><?php $solution_status->re; ?></td></tr>
      <tr class=tre><td><strong>Output Limit Exceed</strong></td><td><?php $solution_status->ole; ?></td></tr>
      <tr class=tro><td><strong>Compile Error</strong></td><td><?php $solution_status->ce; ?></td></tr>
    </tbody></table>
   </div>
  </div>

  <br />

  <div id="mainp">
<?php
	if($rank_num > 0)
	   echo "<span class=bt><a href=problemstatus.php?start=$rank_num - $no_of_rows&problem_id=$problem_id>Prev Page</a></span>&nbsp";

    if($rank_num + $no_of_row < $total_row)
      echo "<span class=bt><a href=problemstatus.php?start=$rank_num + $no_of_rows&problem_id=$problem_id>Next Page</a></span>";
?>
	<br><br>
  </div>

<?php
	include('../include/tailer.php');
?>
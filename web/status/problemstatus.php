<?php
 include('../include/header.php');
 include('../common/tcpclient.php');
 include('../common/config.php');
 include('classes/status_t.php');

 if(isset($_GET['start']))
	 $start = $_GET['start'];
 else
	$start = '0';
 if(isset($_GET['problem_id']))
	 $problem_id = $_GET['problem_id'];
 else
	$problem_id = '0';

 $solution_status = array(); //×ó±ß
 get_solution_status($problem_id, $solution_status);
 if(empty($solution_status)){
	 header("Location: error.php?errorMsg='The problem doesn't exist!'");
	 exit;
 }

 $no_of_row = 25;
 $ps = new status_t($start, $problem_id, '?', '1', '?', '?', 'N', '1' , $user_id?$user_id:'?');
 $ps->getResult();
 $rows = $ps->getRow();

?>


  <div id="tt">
  Problem Status - <a href="../problem/problem.php?problem_id=<?php echo $problem_id; ?>" ><?php echo $problem_id; ?></a>
  </div>
<?php
	include('../include/notice.php');
?>
</center>
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
	  for($i=0, $rank = $start*$no_of_row+1; $i<$rows; $i++, $rank++){
		  if($i%2==0)
			  echo '<tr class=tro>';
		  else
			  echo '<tr class=tre>';

		  echo "<td>$rank</td>";
		  echo "<td>".$ps->getSolution_id($i)."</td>";
		  $user_id = $ps->getUser_id($i);
          echo "<td><a href=../status/userStatus.php?user_id=$user_id>$user_id</a></td>";
          echo "<td>".$ps->getMemory($i)."K</td>";
          echo "<td>".$ps->getTime($i)."MS</td>";
		  echo "<td>";

		  if ($ps->getPermission($i) == 'Y')
			  echo "<a href=../source/source.php?solution_id=".$ps->getSolution_id($i)." target=_blank>".$LANGUAGE[$ps->getLanguage($i)]."</a>";
		  else
			  echo $LANGUAGE[$ps->getLanguage($i)];
		  echo "</td>";
          echo "<td>".$ps->getCode_length($i)."</td>";
          echo "<td>".$ps->getIn_date($i)."</td>";
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
      <tr class=tro><td><strong>User Submitted</strong></td><td><?php echo $solution_status[2]; ?></td></tr>
      <tr class=tre><td><strong>Users Solved</strong></td><td><?php echo $solution_status[3]; ?></td></tr>
      <tr class=tro><td><strong>Total Submits</strong></td><td><?php echo $solution_status[1]; ?></td></tr>
      <tr class=tre><td><strong>Accepted</strong></td><td><?php echo $solution_status[0]; ?></td></tr>
      <tr class=tro><td><strong>Presentation Error</strong></td><td><?php echo $solution_status[4]; ?></td></tr>
      <tr class=tre><td><strong>Time Limit Exceed</strong></td><td><?php echo $solution_status[5]; ?></td></tr>
      <tr class=tro><td><strong>Memory Limit Exceed</strong></td><td><?php echo $solution_status[6]; ?></td></tr>
      <tr class=tre><td><strong>Wrong Answer</strong></td><td><?php echo $solution_status[8]; ?></td></tr>
      <tr class=tro><td><strong>Runtime Error</strong></td><td><?php echo $solution_status[9]; ?></td></tr>
      <tr class=tre><td><strong>Output Limit Exceed</strong></td><td><?php echo $solution_status[7]; ?></td></tr>
      <tr class=tro><td><strong>Compile Error</strong></td><td><?php echo $solution_status[10]; ?></td></tr>
    </tbody></table>
   </div>

  <br />


<?php
	if($start > 0){
		$pre = $start - 1;
	    echo "<span class=bt><a href=problemstatus.php?start=$pre&problem_id=$problem_id>Prev Page</a></span>&nbsp";
    }

    if($rows == 25){
		$next = $start + 1;
      echo "<span class=bt><a href=problemstatus.php?start=$next&problem_id=$problem_id>Next Page</a></span>";
	}
?>
	<br><br>
  </div>

<?php
	include('../include/tailer.php');
?>


<?php
function get_solution_status($problem_id, &$solution_status)
{
	///////////////////////////////
/*	$d = "\001";
	$recv = "12".$d."100".$d."120".$d."23".$d."34".$d."45".$d."0".$d."1".$d."56".$d."21".$d."5";
	$solution_status = explode("\001", $recv);
	return;
*/	////////////////////////////////

	if(empty($problem_id)){
		$solution_status = null;
		return;
	}

	$header = sprintf("%s%08d", "ss", strlen($problem_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$solution_status =  explode("\001", $recv);
	}
	else $solution_status = null;
	$tc->close();
	return;
}
?>
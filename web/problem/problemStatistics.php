<?php
$host = 'localhost';
$user = 'root';
$pass = '123456';
$database = 'flood';
$conn = new mysqli($host, $user, $pass, $database);
$problem_id = (int)$_GET['problem_id'] + 0;
if($problem_id < 1001)die("N/A|N/A");
$query = "SELECT `accepted`, `submit` FROM problems WHERE problem_id={$problem_id}";
$result = $conn->query($query);
$ans = $result->fetch_row();
die($ans[0]."|".$ans[1]);
?>

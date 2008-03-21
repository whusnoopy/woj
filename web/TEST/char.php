<?php
function arr()
{
	$d="\001";
	$message = "magiii".$d."magiii".$d."mcje2004@126.com".$d."wuhan university";
	$message .= $d;
	$message .= "ooy".$d."ooy".$d."ooy@126.com".$d."wuhan university";
	$message .= $d;
	$message .= "www".$d.$d."www@126.com".$d."wuhan university";
	return explode("\001", $message);
}

$arr = arr();
for($i=0; $i<count($arr); $i++){
	echo $arr[$i].":";
	if($i % 4 == 3)
		echo "<br>";
}

?>
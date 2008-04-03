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
$a='12';
$num = 20;
echo $a*$num."<br>";

$solveds = 21;
$submits = 122;
$ratio = sprintf("%.2f%s",$solveds * 100 / $submits, '%');
echo $ratio."<br>";

$s='2';
if($s>1)
echo "'2'>1";
else
echo "'2'<=1";

echo "<br>";

$s='2';
echo "s=".($s+1).'<br>';

$A=41;
echo "\x41";

$d="\001";
$str = $d.'6';
$s= explode($d, $str);
echo count($s);
?>
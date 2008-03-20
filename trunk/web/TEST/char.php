<?php
$d = "\001";
$message = "magiii".$d."magiii".$d."mcje2004@126.com".$d."wuhan university";
$header = sprintf("%s%08d", "rg", strlen($message));
echo $header;
echo "<br>";
echo $message;

?>
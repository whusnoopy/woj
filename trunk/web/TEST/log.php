<?php

$log_file = 'E:\WWW\SERVER\Apache\logs\error.log';

$fp = fopen($log_file, 'r');
$fs = fstat($fp);
$flen = $fs['size'];
fseek($fp, -4096, SEEK_END);
echo '<html><body><pre>';
while(!feof($fp)){
	$line = fgets($fp);
	if ($line{0} != '[') continue;
	if (preg_match("/PHP Fatal/", $line))
		echo '<font color=red>'.$line.'</font>';
	else if(preg_match("/PHP Warning/", $line))
		echo '<font color=blue>'.$line.'</font>';
	else if(preg_match("/PHP Notice/", $line))
		echo '<font color=green>'.$line.'</font>';
	else
		echo $line;
}
echo '</pre></body></html>';
?>
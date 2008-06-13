<?php
//检测目录内的所有文件的行数和字节数
echo '<pre>';
$line = 0;
$filenum = 0;
$byte = 0;

function fileline($filename)
{
	$fp = fopen($filename, 'r') or die('file open failed !');
	$l=0;
	while (!feof($fp)){
		fgets($fp);
		$l++;
	}
	fclose($fp);
	global $line;
	$line += $l;
	return $l;
}

function dirline($url)
{
	if (is_dir($url)){
		$handle = opendir($url);
		while (false != ($file = readdir($handle))){
			if ($file[0] != '.'){

				$file = $url.'/'.$file;
				if (is_file($file)){ //&& strrchr($file, '.') == '.php'){
					global $filenum, $byte;
					$filenum++;
					$byte += filesize($file);
					printf("%-60s%d\n", $file,fileline($file));
				}
				else
					dirline($file);
			}
		}
		closedir($handle);
	}
}

$dir = 'D:/flood/server';
dirline($dir);
echo "<pre>\nsumary:\n";
echo "there are total $filenum files\n";
echo "there are tolal $line lines\n";
echo "there are total $byte bytes\n";
echo '</pre>';
?>
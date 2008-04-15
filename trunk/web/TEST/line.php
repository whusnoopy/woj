<?php
//检测目录内的所有文件的行数和字节数

$line = 0;
$filenum = 0;
$byte = 0;

function fileline($filename)
{
	$fp = fopen($filename, 'r') or die('file open failed !');
	$l=0;
	while (!feof($fp)){
		global $byte;
		$byte += strlen(fgets($fp));
		$l++;
	}
	global $line;
	$line += $l;
	return $l;
}

function dirline($url)
{
	if (is_dir($url)){
		$handle = opendir($url);
		while (false != ($file = readdir($handle))){
			if ($file != '.' && $file != '..'){

				$file = $url.'/'.$file;
				if (is_file($file)){
					global $filenum;
					$filenum++;
					printf("%-60s%d\n", $file,fileline($file));
				}
				else
					dirline($file);
			}
		}
		closedir($handle);
	}
}

$dir = 'E:\www\web';
dirline($dir);
echo "\nsumary:\n";
echo "there are total $filenum files\n";
echo "there are tolal $line lines\n";
echo "there are total $byte bytes\n";
?>
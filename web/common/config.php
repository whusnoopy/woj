<?php

function readconfig($request)
{
	$fd = fopen("flood.conf", 'r');
	if(!$fd) return null;
	while (!feof($fd)) {
		$buffer = fgets($fd, 4096);
		sscanf($buffer, "%s : %s", $param, $value);
		if (strcasecmp($param, $request) == 0){
			fclose($fd);
			return $value;
		}
	}
	fclose($fd);
	return null;
}

//echo readconfig("HOST");
//echo readconfig("PORT");
?>

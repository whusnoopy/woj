<?php
function gen_html($filename)
{
	ob_start();
	$require = $filename.'.php';
	require_once $require;
	$buf = ob_get_clean();
	$fp = fopen($filename.'.html', 'w') or die("open file $filename failed!");
	fwrite($fp, $buf);
	fclose($fp);
}


gen_html('contest/contestList');
gen_html('faq');

?>
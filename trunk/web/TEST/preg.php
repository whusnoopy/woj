<?php


$strs = array('1234', '012', '', '1234567', 'aaa', '01a', 'a01', '_0', '0.1', '000000');
forEach ($strs as $str){
	echo $str.'  :  ';
	if (preg_match('/^\d{1,6}$/', $str))
		echo "match\n";
	else
		echo "no\n";
}

?>

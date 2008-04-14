<?php
function get_ip()
{
	if($_SERVER['HTTP_CLIENT_IP']){
		 $ip = $_SERVER['HTTP_CLIENT_IP'];
	}else if($_SERVER['HTTP_X_FORWARDED_FOR']){
		$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
	}else if ($_SERVER['REMOTE_ADDR']){
		$ip = $_SERVER['REMOTE_ADDR'];
	}
	else
		$ip = 'unknow';
	return $ip;
}
?>
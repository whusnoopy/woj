<?php
function get_ip()
{
	if(getenv('HTTP_CLIENT_IP')){
		 $ip = getenv('HTTP_CLIENT_IP');
	}else if(getenv('HTTP_X_FORWARDED_FOR')){
		$ip = getenv('HTTP_X_FORWARDED_FOR');
	}else if ($_SERVER['REMOTE_ADDR']){
		$ip = $_SERVER['REMOTE_ADDR'];
	}
	else
		$ip = 'unknow';
	return $ip;
}
?>
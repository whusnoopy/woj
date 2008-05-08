<?php
function get_ip()
{
	if ($ip = getenv('HTTP_CLIENT_IP')){
		 ;
	}else if ($ip = getenv('HTTP_X_FORWARDED_FOR')){
		;
	}else if ($ip = $_SERVER['REMOTE_ADDR']){
		;
	}
	else
		$ip = 'unknow';
	return $ip;
}
?>
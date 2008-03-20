<?php
	include_once ('tcpclient.php');

class new_mail{

	var $m_user_id;

	function new_mail($user_id = '')
	{
		$this->m_user_id = $user_id;
	}
	function check_new_mail()
	{
		//just for test
		return true;

		$host = readconfig ("host");
		$port = readconfig ("port");

		$client = new TCPClient($host, $port);
		$client->create();
		$client->connect();
		$client->sendstr('new_mail'.':'.$this->m_user_id);
		if($client->readstr(1) == 'Y')
			return true;
		else
			return false;
		$client->close();
	}
}
?>

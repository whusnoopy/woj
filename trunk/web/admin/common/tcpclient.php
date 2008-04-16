<?php
	include('config.php');
?>

<?php

class TCPClient{
	var $m_host;
	var $m_port;
	var $m_socket;

	function TCPClient()
	{
		$this->m_host = "192.168.5.187";
		$this->m_port = 3030;
	}
	function set_host($host)
	{
		$this->m_host = $host;
	}
	function set_port($port)
	{
		$this->m_port = $port;
	}

	function create()
	{
		if(!($this->m_socket = socket_create(AF_INET, SOCK_STREAM, 0)) ){
			return false;
		}
	//	socket_set_nonblock($this->m_socket);
		return true;
	}
	function connect()
	{
		if(!socket_connect($this->m_socket, $this->m_host, $this->m_port)){
			return false;
		}
		return true;
	}
	function close()
	{
		socket_close($this->m_socket);
	}

	function sendstr($str)
	{
		$ret = socket_write($this->m_socket,$str);
		if(!$ret || $ret < strlen($str))
			return false;
	    return true;
	}
	function recvstr($len)
	{
		return socket_read($this->m_socket, $len);

	}

}

?>
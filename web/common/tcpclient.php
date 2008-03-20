
<?php

class TCPClient{
	var $m_host;
	var $m_port;
	var $m_socket;

	function TCPClient()
	{
		$this->m_host = "192.168.5.183";
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
		$ret = socket_read($this->m_socket, $len);
		return $ret;
	}
	function sendfile($filename)
	{
		$fp = fopen($filename, "r");
		if ($fp){
			return false;
		}
		while (true){
			$data = fread($fp, 128);
       		if (strlen($data) == 0) {
         		break;
       		}
       		$this->sendstr($data);
		}
		fclose($fp);
		return true;
	}
}


/*
  $client = new TCPClient("127.0.0.1", 1234);
  $client->create();
 // echo "created";
  $client->connect();
//  echo "connected";

  $fp = fopen("logo.gif", "r");

  do {
       $data = fread($fp, 128);
       if (strlen($data) == 0) {
         break;
       }
//	   echo $data;
       $client->writesock($data);
       } while (true);
  fclose($fp);

//  echo "sent";
  $receive = $client->readsock(1000);
  $client->close();
  echo $receive;
*/
?>

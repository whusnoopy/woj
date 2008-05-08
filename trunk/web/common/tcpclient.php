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
/*
function get_problem_info($problem_id, &$problem)
{
	if(empty($problem_id)){
		$problem = null;
		return;
	}

	$header = sprintf("%s%08d", "pb", strlen($problem_id));

	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	$len = sscanf($recv, "%d");
	if($len > 0){
		$recv = $tc->recvstr($len);
		$problem =  explode("\001", $recv);
	}
	else $problem = null;
	$tc->close();
	return;
}
*/
/*
$tc = new TCPClient();
$tc->create();
$tc->connect();
$tc->sendstr("sfsafasd");
*/
?>
<?php

class user_list_t
{
	var $pagenum;
	var $user_id;
	var $result = array();

	function user_list_t($pagenum = '0', $user_id = '?')
	{
		$this->pagenum = $pagenum;
		$this->user_id = $user_id;
	}

	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}
	function setUser_id($user_id)
	{
		$this->user_id = $user_id;
	}

	function getUser_id($i)
	{
		return $this->result[$i*5];
	}
	function getNick($i)
	{
		return $this->result[$i*5 + 1];
	}
	function getLast_login_ip($i)
	{
		return $this->result[$i*5 + 2];
	}
	function getLast_login_time($i)
	{
		return $this->result[$i*5 + 3];
	}
	function getRegister_time($i)
	{
		return $this->result[$i*5 + 4];
	}

    function getRow()
	{
		return count($this->result) / 5;
	}

	function getResult()
	{
		$d = "\001";
		$message = $this->user_id.$d.'?'.$d.$this->pagenum;
		$header = sprintf("%s%08d", "ul", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		$tc->connect() or die("unable to connect to server!");
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message)or die("send message failed");
		$recv= $tc->recvstr(10);
		sscanf($recv, "%d", $len);
		if($len > 0){
			$recv = $tc->recvstr($len);
			$this->result = explode($d, $recv);
		}
		else
			$this->result = null;
		$tc->close();

	}
}

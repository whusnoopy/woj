<?php

class mail_list_t
{
	var $user_id;
	var $pagenum;
	var $result = array();

	function mail_list_t($user_id, $pagenum)
	{
		$this->user_id = $user_id;
		$this->pagenum = $pagenum;
	}

	function setUser_id($user_id)
	{
		$this->user_id = $user_id;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function getMail_id($i)
	{
		return $this->result[$i*6];
	}
	function getTo_user($i)
	{
		return $this->result[$i*6 + 1];
	}
	function getFrom_user($i)
	{
		return $this->result[$i*6 + 2];
	}
	function getTitle($i)
	{
		return $this->result[$i*6 + 3];
	}
	function getIn_date($i)
	{
		return $this->result[$i*6 + 4];
	}
	function getRead($i)
	{
		return $this->result[$i*6 + 5];
	}

	function getRow()
	{
		return count($this->result) / 6;
	}

	function getResult()
	{
		///////////////////////////
/*		$d = "\001";
		$recv = "1".$d."1001".$d."a+b".$d."...".$d."60".$d."100".$d.
				"2".$d."1002".$d."www".$d."...".$d."20".$d."53";
		$this->result = explode($d, $recv);
		return;
*/		///////////////////////////


		$d = "\001";
		$message = $this->user_id.$d.$this->pagenum;
		$header = sprintf("%s%08d", "ml", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		if (!$tc->connect()){// or die("unable to connect to server!");
			header('HTTP/1.1 404 Not Found');
			exit;
		}
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


?>
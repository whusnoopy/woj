<?php
//include_once('../../common/tcpclient.php');

class problem_list_t
{

	var $pagenum;
	var $user_id;
	var $title;
	var $source;
	var $of1;
	var $result = array();

	function problem_list_t($pagenum='0', $user_id='?', $title='?', $source='?')
	{
		$this->pagenum = $pagenum;
		$this->user_id = $user_id;
		$this->title = $title;
		$this->source = $source;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}
	function setUser_id($user_id)
	{
		$this->user_id = $user_id;
	}
	function setTitle($title)
	{
		$this->title = $title;
	}
	function setSource($source)
	{
		$this->source = $source;
	}


	function getPages()
	{
		return $this->result[0];
	}
	function getProblem_id($i)
	{
		return $this->result[$i*5 + 1];
	}
	function getTitle($i)
	{
		return $this->result[$i*5 + 2];
	}
	function getAC($i)
	{
		return $this->result[$i*5 + 3];
	}
	function getTotal($i)
	{
		return $this->result[$i*5 + 4];
	}
	function getResult_type($i)
	{
		return $this->result[$i*5 + 5];
	}

	function getRow()
	{
		return (count($this->result)-1) / 5;
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
		$message = $this->pagenum.$d.'0'.$d.$this->title.$d.$this->source.$d.'0'.$d.$this->user_id;
		$header = sprintf("%s%08d", "pl", strlen($message));

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

/*
$pl = new problem_list_t();
$pl->getResult();
*/
?>
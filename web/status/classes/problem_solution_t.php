<?php

class problem_solution_t
{
	//Property

	var $problem_id;
    var $pagenum;
	var $valid_array = array();
	var $result;

	//construction
	function problem_solution_t($problem_id, $pagenum)
	{
		$this->problem_id = $problem_id;
		$this->pagenum = $pagenum;
	}


	function setProblem_id($problem_id)
	{
		$this->problem_id = $problem_id;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function getSolution_id($i)
	{
		return $this->result[$i*7];
	}
	function getUser_id($i)
	{
		return $this->result[$i*7+1];
	}
	function getMemory($i)
	{
		return $this->result[$i*7+2];
	}
	function getTime($i)
	{
		return $this->result[$i*7+3];
	}
	function getLanguage($i)
	{
		return $this->result[$i*7+4];
	}
	function getCode_length($i)
	{
		return $this->result[$i*7+5];
	}
	function getIn_date($i)
	{
		return $this->result[$i*7+6];
	}


	function getRow()
	{
		return count($this->result) / 7;
	}
	function getResult()
	{

		///////////////////////////////
		$d = "\001";
		$recv = '10634'.$d.'magiii'.$d.'72'.$d.'0'.$d.'GCC'.$d.'122'.$d.'2006-05-04 05:41:19.0'.$d.
				'10634'.$d.'kittyshow'.$d.'75'.$d.'0'.$d.'GCC'.$d.'160'.$d.'2006-05-05 05:41:19.0';
		$this->result = explode("\001", $recv);
		return;
		////////////////////////////////

		$d = "\001";
		$message = $this->problem_id.$d.$this->pagenum;
		$header = sprintf("%s%08d", "ps", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		$tc->connect() or die("unable to connect to server!");
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message)or die("send message failed");
		$recv= $tc->recvstr(10);
		$len = sscanf($recv, "%d");
		if($len > 0){
			$recv = $tc->recvstr($len);
			$this->result = explode($d, $recv);
		}
		else
			$this->result = null;
		$tc->close;
	}


	function is_valid($i)
	{
		return false;
		return $this->valid_array[$i];
	}

}

?>
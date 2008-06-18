<?php
//include_once('../../common/tcpclient.php');

class contest_statistics_t
{
	var $contest_id;
	var $result = array();

	function contest_statistics_t($contest_id)
	{
		$this->contest_id = $contest_id;
	}
	function setContest_id($contest_id)
	{
		$this->contest_id = $contest_id;
	}

	function getProblem_id($i)
	{
		return $this->result[$i*13];
	}
	function getAC($i)
	{
		return $this->result[$i*13+1];
	}
	function getPE($i)
	{
		return $this->result[$i*13+2];
	}
	function getCE($i)
	{
		return $this->result[$i*13+3];
	}
	function getWA($i)
	{
		return $this->result[$i*13+4];
	}
	function getTLE($i)
	{
		return $this->result[$i*13+5];
	}
	function getRE($i)
	{
		return $this->result[$i*13+6];
	}
	function getMLE($i)
	{
		return $this->result[$i*13+7];
	}
	function getOLE($i)
	{
		return $this->result[$i*13+8];
	}
	function getTotal($i)
	{
		return $this->result[$i*13+9];
	}
	function getC_CPP($i)
	{
		return $this->result[$i*13+10];
	}
	function getJAVA($i)
	{
		return $this->result[$i*13+11];
	}
	function getPASCAL($i)
	{
		return $this->result[$i*13+12];
	}

	function getRow()
	{
		return count($this->result) / 13;
	}

	function getResult()
	{
		/////////////////////////////
/*		$d="\001";
		$recv = '1011'.$d.'2'.$d.'0'.$d.'12'.$d.'13'.$d.'11'.$d.'13'.$d.'4'.$d.'0'.$d.'55'.$d.'47'.$d.'3'.$d.'5';
		$recv .= $d.'1013'.$d.'3'.$d.'2'.$d.'2'.$d.'34'.$d.'11'.$d.'23'.$d.'6'.$d.'7'.$d.'6'.$d.'34'.$d.'34'.$d.'2';
		$this->result = explode($d, $recv);
		return;
*/		//////////////////////////////////

		$d = "\001";
		$message = $this->contest_id;
		$header = sprintf("%s%08d", "cs", strlen($message));

		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		if (!$tc->connect()){ // or die("unable to connect to server!");
			header('HTTP/1.1 404 Not Found');
			exit;
		}
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message)or die("send message failed");
		$recv = $tc->recvstr(10);

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
$pl = new contest_statistics_list_t();
$pl->getResult();
*/
?>
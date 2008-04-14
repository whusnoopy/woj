<?php
//include_once('../../common/tcpclient.php');

class contest_list_t
{
	var $pagenum;
	var $result = array();

	function contest_list_t($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function getID($i)
	{
		return $this->result[$i*7];
	}
	function getTitle($i)
	{
		return $this->result[$i*7+1];
	}
	function getStart_time($i)
	{
		return $this->result[$i*7+2];
	}
	function getEnd_time($i)
	{
		return $this->result[$i*7+3];
	}
	function getState($i)
	{
		$start = strtotime($this->result[$i*6+2]);
		$end = strtotime($this->result[$i*6+3]);
		$current = time()+8*3600;

		if($current > $end)
			return 'Finished';
		else if($current >= $start)
			return 'Running';
		else
			return 'Scheduled';
	}
	function get_type($i)
	{
		return $this->result[$i*7+4];
	}
	function getPublic_id($i)
	{
		return $this->result[$i*7+5];
	}
	function isAvailable($i)
	{
		if ($this->result[$i*7+6] =='Y')
			return true;
		return false;
	}

	function getRow()
	{
		return count($this->result)/7;
	}
	function getResult()
	{
		/////////////////////////////
/*		$d="\001";
		$recv = "1001".$d."hello, magiii".$d."2008-03-23 18:30:00".$d."2008-03-23 23:30:00".$d."public".$d."1".$d."Y"
			.$d."1002".$d."hello, snoopy".$d."2008-03-25 18:30:00".$d."2008-03-31 18:30:00".$d."public".$d."1".$d."N"
			.$d."1003".$d."hello, ooyean".$d."2008-04-23 18:30:00".$d."2008-05-23 23:30:00".$d."public".$d."1".$d."Y";
		$this->result = explode($d, $recv);
		return;
*/		//////////////////////////////////

		$d = "\001";
		$message = $this->pagenum;
		$header = sprintf("%s%08d", "rc", strlen($message));

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
		$tc->close;
	}
}


?>

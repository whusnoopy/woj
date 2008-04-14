<?php
class ranklist_t
{
	var $pagenum;
	var $of1;
	var $result = array();

	function ranklist_t($pagenum='0', $of1='0')
	{
		$this->pagenum = $pagenum;
		$this->of1 = $of1;
	}

    function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function setOf1($of1)
	{
		$this->of1 = $of1;
	}

	function getUser_id($i)
	{
		return $this->result[$i*4];
	}
	function getNick($i)
	{
		return $this->result[$i*4+1];
	}
	function getSolveds($i)
	{
		return $this->result[$i*4+2];
	}
	function getSubmits($i)
	{
		return $this->result[$i*4+3];
	}
	function getRow()
	{
		return count($this->result) / 4;
	}

	function getResult()
	{
		////////////////////////////////////
/*		if($this->pagenum > 0)
			$this->result = null;
		else
			$this->result = array("magiii", "magiii", "100", "121");
		return;
*/		///////////////////////////////////

		$d = "\001";
		$message = $this->pagenum.$d.$this->of1;
		$header = sprintf("%s%08d", "rk", strlen($message));

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

/*
$rk = new ranklist_t();
echo $rk->getRow();
*/

?>
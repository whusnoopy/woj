<?php
//include_once('../../common/tcpclient.php');

class admin_problem_list_t
{

	var $pagenum;
	var $result = array();

	function admin_problem_list_t($pagenum='0')
	{
		$this->pagenum = $pagenum;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}



	function getProblem_id($i)
	{
		return $this->result[$i*5];
	}
	function getTitle($i)
	{
		return $this->result[$i*5+1];
	}
	function getAC($i)
	{
		return $this->result[$i*5+2];
	}
	function getTotal($i)
	{
		return $this->result[$i*5+3];
	}
	function getAvailable($i)
	{
		return $this->result[$i*5+4];
	}

	function getRow()
	{
		return count($this->result) / 5;
	}

	function getResult()
	{
		///////////////////////////
/*		$d = "\001";
		$recv = "1001".$d."a+b".$d."60".$d."100".$d.'Y'.$d.
				"1002".$d."www".$d."20".$d."53".$d.'N';
		$this->result = explode($d, $recv);
		return;
*/		///////////////////////////


		$d = "\001";
		$message = $this->pagenum;
		$header = sprintf("%s%08d", "rp", strlen($message));

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

/*
$pl = new problem_list_t();
$pl->getResult();
*/
?>
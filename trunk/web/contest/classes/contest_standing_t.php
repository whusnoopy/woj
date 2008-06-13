<?php
//include_once('../../common/tcpclient.php');

class contest_standing_t
{
	var $contest_id;
	var $pagenum;
	var $problem_num;
	var $result = array();

	function contest_standing_t($contest_id, $pagenum)
	{
		$this->contest_id = $contest_id;
		$this->pagenum = $pagenum;
		$this->problem_num = 0;
	}
	function setContest_id($contest_id)
	{
		$this->contest_id = $contest_id;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function getProblem_num()
	{
		return $this->problem_num;

	}
	function getUser_id($i)
	{
		return $this->result[1 + $i * (4 + $this->problem_num * 2)];
	}
	function getNick($i)
	{
		return $this->result[2 + $i * (4 + $this->problem_num * 2)];
	}
	function getAC($i)
	{
		return $this->result[3 + $i * (4 + $this->problem_num * 2)];
	}
	function getPenalty($i)
	{
		return $this->result[4 + $i * (4 + $this->problem_num * 2)];
	}
	function getProblem_penalty($i, $j)
	{
		return $this->result[5 + $i * (4 + $this->problem_num * 2) + $j * 2];
	}
	function getProblem_submit($i, $j)
	{
		return $this->result[5 + $i * (4 + $this->problem_num * 2) + $j * 2 + 1];
	}

	function getRow()
	{
		return (count($this->result) - 1) / ($this->problem_num * 2 + 4);
	}

	function getResult()
	{
		/////////////////////////////
/*		$d="\001";
		 $recv = '4'.$d.'magiii'.$d.'aaaaa'.$d.'4'.$d.'16:44:20'.$d.'00:17:04'.$d.'0'.$d.'02:53:20'.$d.'0'.$d.'00:36:26'.$d.'0'.$d.'02:11:48'.$d.'0'.$d.	             'ooyean'.$d.'bbbbb'.$d.'4'.$d.'17:40:44'.$d.'00:07:05'.$d.'0'.$d.'01:45:31'.$d.'(-2)'.$d.'02:35:45'.$d.'0'.$d.'01:08:26'.$d.'(-2)';
		$this->result = explode($d, $recv);
		$this->problem_num = intval($this->result[0]);
		return;
*/		//////////////////////////////////

		$d = "\001";
		$message = $this->contest_id.$d.$this->pagenum;
		$header = sprintf("%s%08d", "cr", strlen($message));

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

		$this->problem_num = intval($this->result[0]);
	}

}

/*
$pl = new contest_statistics_list_t();
$pl->getResult();
*/
?>
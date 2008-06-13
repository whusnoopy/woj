<?php
//include_once('../../common/tcpclient.php');

class contest_problem_list_t
{
	var $contest_id;
	var $user_id;
	var $result = array();

	function contest_problem_list_t($contest_id, $user_id = '?')
	{
		$this->contest_id = $contest_id;
		$this->user_id = $user_id;
	}
	function setContest_id($contest_id)
	{
		$this->contest_id = $contest_id;
	}
	function setUser_id($user_id)
	{
		$this->user_id = $user_id;
	}


	function getProblem_id($i)
	{
		return $this->result[$i*5];
	}
	function getTitle($i)
	{
		return $this->result[$i*5+2];
	}
	function getAC($i)
	{
		return $this->result[$i*5+3];
	}
	function getTotal($i)
	{
		return $this->result[$i*5+4];
	}

	function getRow()
	{
		return count($this->result) / 5;
	}

	function getResult()
	{
		/////////////////////////////
/*		$d="\001";
		$recv = "1001".$d."a+b".$d."12".$d."19"
			.$d."1002".$d."ssssss".$d."14".$d."56"
			.$d."1003".$d."huoxing".$d."3".$d."123";
		$this->result = explode($d, $recv);
		return;
*/		//////////////////////////////////

		$d = "\001";
		$message = $this->contest_id.$d.$this->user_id;
		$header = sprintf("%s%08d", "cp", strlen($message));


		$tc = new TCPClient();
		$tc->create() or die("unable to create socket!");
		$tc->connect() or die("unable to connect to server!");
		$tc->sendstr($header) or die("send header failed");
		$tc->sendstr($message)or die("send message failed");
		$recv = $tc->recvstr(1);
		if ($recv != 'Y'){
			$this->result = null;
			$tc->close();
			return;
		}
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
$pl = new contest_problem_list_t();
$pl->getResult();
*/
?>
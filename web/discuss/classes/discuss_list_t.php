<?php
	include_once('../../common/tcpclient.php');

class discuss_list_t
{
	var $title;
	var $problem_id;
	var $contest_id;
	var $user_id;
	var $pagenum;
	var $result = array();

	function discuss_list_t($title, $problem_id, $contest_id, $user_id, $pagenum)
	{
		$this->title = $title;
		$this->problem_id = $problem_id;
		$this->contest_id = $contest_id;
		$this->user_id = $user_id;
		$this->pagenum = $pagenum;
	}

	function setTitle($title)
	{
		$this->title = $title;
	}
	function setProblem_id($problem_id)
	{
		$this->problem_id = $problem_id;
	}
	function setContest_id($contest_id)
	{
		$this->contest_id = $contest_id;
	}
	function setUser_id($user_id)
	{
		$this->user_id = $user_id;
	}
	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}

	function getLevel($i)
	{
		return $this->result[$i*7];
	}
	function getDiscuss_id($i)
	{
		return $this->result[$i*7 + 1];
	}
	function getTitle($i)
	{
		return $this->result[$i*7 + 2];
	}
	function getIn_date($i)
	{
		return $this->result[$i*7 + 3];
	}
	function getUser_id($i)
	{
		return $this->result[$i*7 + 4];
	}
	function getProblem_id($i)
	{
		return $this->result[$i*7 + 5];
	}
	function getContest_id($i)
	{
		return $this->result[$i*7 + 6];
	}

	function getRow()
	{
		return count($this->result) / 7;
	}
	function getResult()
	{
		/////////////////////////////
		$d="\001";
		$recv = "1".$d."1001".$d."afasdfasfaegagagadg".$d."2008-03-23 18:30:00".$d."magiii".$d."1001".$d."1001"
			.$d."2".$d."1002".$d."Re: agrgraegadfgagaregaergaer".$d."2008-03-25 18:30:00".$d."snoopy".$d."1001".$d."1001"
			.$d."3".$d."1003".$d."Re: Re: agragargargaegaergaergrg".$d."2008-04-23 18:30:00".$d."ooeyean".$d."1001".$d."1001";
		$recv .= $d."1".$d."1004".$d."sadASdcsafexcvsdvs cAdwf".$d."2008-03-20 18:30:00".$d."magiii".$d."1002".$d."1001";
		$this->result = explode($d, $recv);
		return;
		//////////////////////////////////

		$d = "\001";
		$message = $this->title.$d.$this->problem_id.$d.$this->contest_id.$d.$this->user_id.$d.$this->pagenum;
		$header = sprintf("%s%08d", "dl", strlen($message));

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
}



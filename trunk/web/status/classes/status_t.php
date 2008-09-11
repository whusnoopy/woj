<?php

class status_t
{
	var $pagenum;
	var $problem_id;
	var $user_id;
	var $rst;
	var $language;
	var $contest_id;
	var $share_code;
	var $type;
	var $cur_user_id;

	var $result = array();

	function status_t($pagenum, $problem_id='0', $user_id='?', $rst='?',
		$language='?', $contest_id='?', $share_code='N', $type='0', $cur_user_id='?')
	{
		$this->pagenum = $pagenum;
		$this->problem_id = $problem_id;
		$this->user_id = $user_id;
		$this->rst = $rst;
		$this->language = $language;
		$this->contest_id = $contest_id;
		$this->share_code = $share_code;
		$this->type = $type;
		$this->cur_user_id = $cur_user_id;
	}

	function setPagenum($pagenum)
	{
		$this->pagenum = $pagenum;
	}
	function setProblem_id($problem_id)
	{
		$this->problem_id = $problem_id;
	}
	function setUser_id($puser_id)
	{
		$this->user_id = $user_id;
	}
	function setRst($rst)
	{
		$this->rst = $rst;
	}
	function setLanguage($language)
	{
		$this->language = $language;
	}
	function setContest_id($contest_id)
	{
		$this->contest_id = $contest_id;
	}
	function setShare_code($share_code)
	{
		$this->share_code = $share_code;
	}
	function set_Type($type)
	{
		$this->type = $type;
	}
	function setCur_user_id($cur_user_id)
	{
		$this->cur_user_id = $cur_user_id;
	}

	function getSolution_id($i)
	{
		return $this->result[$i*12];
	}
	function getUser_id($i)
	{
		return $this->result[$i*12 + 1];
	}
	function getProblem_id($i)
	{
		return $this->result[$i*12 + 2];
	}
	function getRst($i)
	{
		return $this->result[$i*12 + 3];
	}
	function getMemory($i)
	{
		return $this->result[$i*12 + 4];
	}
	function getTime($i)
	{
		return $this->result[$i*12 + 5];
	}
	function getLanguage($i)
	{
		return $this->result[$i*12 + 6];
	}
	function getCode_length($i)
	{
		return $this->result[$i*12 + 7];
	}
	function getIn_date($i)
	{
		return $this->result[$i*12 + 8];
	}
	function getCode_id($i)
	{
		return $this->result[$i*12 + 9];
	}
	function getError_id($i)
	{
		return $this->result[$i*12 + 10];
	}
	function getPermission($i)
	{
		return $this->result[$i*12 + 11];
	}

	function getRow()
	{
		return count($this->result) / 12;
	}

	function getResult()
	{

		///////////////////////////////
/*		$d = "\001";
		$recv = '10634'.$d.'magiii'.$d.'1002'.$d.'AC'.$d.'72'.$d.'0'.$d.'GCC'.$d.'122'.$d.'2006-05-04 05:41:19.0'.$d.'1024'.$d.'Y'.$d.
				'10634'.$d.'kittyshow'.$d.'1002'.$d.'AC'.$d.'75'.$d.'0'.$d.'GCC'.$d.'160'.$d.'2006-05-05 05:41:19.0'.$d.'1024'.$d.'N';
		$this->result = explode("\001", $recv);
		return;
*/		////////////////////////////////

		$d = "\001";
		$message = $this->pagenum
			.$d.$this->problem_id
			.$d.$this->user_id
			.$d.$this->rst
			.$d.$this->language
			.$d.$this->contest_id
			.$d.$this->share_code
			.$d.$this->type
			.$d.$this->cur_user_id;

		$header = sprintf("%s%08d", "st", strlen($message));

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

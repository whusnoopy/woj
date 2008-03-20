<?php

class status_t
{
	var $problem_id;
	var $user_id;
	var $result;
	var $language;
	var $contest_id;
	var $startRow;
	var $noOfRow;
	var $tatus;
	var $totalRow;


	public function setProblem_id($problem_id){
		$this->problem_id = $problem_id;
	}

	public function setUser_id($user_id){
		$this->user_id = $user_id;
	}

	public function setResult($result){
		$this->result = $result;
	}

	public function setLanguage($language){
		$this->language = $language;
	}

	public function setContest_id($contest_id){
		$this->contest_id = $contest_id;
	}

	public function setStartRow($startRow){
		$this->startRow = $startRow;
		if($this->startRow == null || strlen($this->startRow) == 0){
			$this->startRow = "0";
		}
	}

	public function setNoOfRow($noOfRow){
		$this->noOfRow = $noOfRow;
	}

	public function getStatus()
	{
		$result = null;
/*		sql.append("select solution_id,user_id,problem_id,result,memory,time,language,code_length,in_date ")
			.append("from solution ");

		boolean first = true;
		if(problem_id != null && problem_id.length() != 0){
			sql.append("where problem_id = ? ");
			first = false;
		}
		if(user_id != null && user_id.length() != 0){
			if(first) sql.append("where user_id = ? ");
			else sql.append("and user_id = ? ");
			first = false;
		}
		if(result != null && result.length() != 0){
			if(first) sql.append("where result = ? ");
			else sql.append("and result = ? ");
			first = false;
		}
		if(language != null && language.length() != 0){
			if(first) sql.append("where language = ? ");
			else sql.append("and language = ? ");
			first = false;
		}
		if(contest_id != null && contest_id.length() != 0){
			if(first) sql.append("where contest_id = ? ");
			else sql.append("and contest_id = ? ");
			first = false;
		}

		sql.append("order by solution_id desc ");
        sql.append("limit "+startRow+","+noOfRow);

		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();
		if(problem_id != null && problem_id.length() != 0) values.add(problem_id);
		if(user_id != null && user_id.length() != 0) values.add(user_id);
		if(result != null && result.length() != 0) values.add(result);
		if(language != null && language.length() != 0) values.add(language);
		if(contest_id != null && contest_id.length() != 0) values.add(contest_id);
		sqlCommandBean.setValues(values);
		Result result = null;
		try{
		   result = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){
			throw new ServletException("Database error"+sql.toString(),e);
		}
		finally{
			try{
				conn.close();
			}
			catch(SQLException e){
				throw new ServletException("Connection closing error" ,e);
			}
		}
		sqlString = sql.toString();
*/
		return $result;
	}


	public function getTotalRow()
	{
/*		throws SQLException,ServletException{
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();

		sql.append("select count(*) as count from solution ");
		boolean first = true;
		if(problem_id != null && problem_id.length() != 0){
			sql.append("where problem_id = ? ");
			first = false;
		}
		if(user_id != null && user_id.length() != 0){
			if(first) sql.append("where user_id = ? ");
			else sql.append("and user_id = ? ");
			first = false;
		}
		if(result != null && result.length() != 0){
			if(first) sql.append("where result = ? ");
			else sql.append("and result = ? ");
			first = false;
		}
		if(language != null && language.length() != 0){
			if(first) sql.append("where language = ? ");
			else sql.append("and language = ? ");
			first = false;
		}
		if(contest_id != null && contest_id.length() != 0){
			if(first) sql.append("where contest_id = ? ");
			else sql.append("and language = ? ");
			first = false;
		}


		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();
		if(problem_id != null && problem_id.length() != 0) values.add(problem_id);
		if(user_id != null && user_id.length() != 0) values.add(user_id);
		if(result != null && result.length() != 0) values.add(result);
		if(language != null && language.length() != 0) values.add(language);
		if(contest_id != null && contest_id.length() != 0) values.add(contest_id);
		sqlCommandBean.setValues(values);
		Result result = null;
		try{
			result = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){
			throw new ServletException("Database error"+sql.toString(),e);
		}
		finally{
			try{conn.close();}
			catch(SQLException e){}
		}
		return Integer.parseInt(((result.getRows()[0]).get("count")).toString());
	*/
		return 12;
	}
}

	$status = new status_t;
	$status->setProblem_id('1001');
	echo $status->getTotalRow();

?>
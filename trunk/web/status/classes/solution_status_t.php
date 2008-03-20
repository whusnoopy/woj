<?php

//返回统计信息,如果problem_id不存在,所有为-1

class solution_status_t
{
	var $problem_id = null;

	var $userSubmited = null;
	var $userSolved = null;
	var $totalSubmits = null;
	var $accepted = null;
	var $pe = null;
	var $tle = null;
	var $mle = null;
	var $ole = null;
	var $wa = null;
	var $re = null;
	var $ce = null;
	var $problemMap = null;

	//construction
	function solution_status_t($problem_id){
		$this->problem_id = $problem_id;
	}

	//set problem_id;
	function set_problem_id($problem_id){
		$this->problem_id = $problem_id;
	}

	function initialize(){
	}
	/*
	public int getAccepted(){
		if(problemMap == null) return 0;
		return Integer.parseInt(problemMap.get("accepted").toString());
	}

	public int getTotalSubmits(){
		if(problemMap == null) return 0;
		return Integer.parseInt(problemMap.get("totalSubmits").toString());
	}
	public int getUserSubmited()
		throws SQLException,ServletException{
		return getSolutionCount(-1,0);
	}

	public int getUserSolved()
		throws SQLException,ServletException{
		return getSolutionCount(0,0);
	}

	public int getPe()
		throws SQLException,ServletException{
		return getSolutionCount(1,1);
	}

	public int getTle()
		throws SQLException,ServletException{
		return getSolutionCount(2,1);
	}

	public int getMle()
		throws SQLException,ServletException{
		return getSolutionCount(3,1);
	}

	public int getOle()
		throws SQLException,ServletException{
		return getSolutionCount(5,1);
	}

	public int getWa()
		throws SQLException,ServletException{
		return getSolutionCount(4,1);
	}

	public int getRe()
		throws SQLException,ServletException{
		return getSolutionCount(7,1) + getSolutionCount(8,1)
			+getSolutionCount(9,1) + getSolutionCount(10,1)
			+getSolutionCount(11,1);
	}

	public int getCe()
		throws SQLException,ServletException{
		return getSolutionCount(5,1);
	}

	private void initialProblemMap()
		throws SQLException,ServletException{
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		if(problem_id == null || problem_id.length() == 0){
			problem_id = new String("0");
		}
		sql.append("select accepted,submit as totalSubmits ")
			.append(" from problem where problem_id = ?");

		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();
		values.add(problem_id);
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
				throw new ServletException("Connection closing error",e);
			}
		}
		if(result.getRowCount() < 1) problemMap = null;
		else problemMap = result.getRows()[0];
	}

	private int getSolutionCount(int num,int type)
		throws SQLException,ServletException{
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		if(type == 0){
			sql.append("select count(distinct(user_id)) as count ")
				.append(" from solution where problem_id = ? ");
		}
        else{
			sql.append("select count(*) as count from solution where problem_id = ? ");
		}
		if(num != -1) sql.append(" and result = ? ");
		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();
		values.add(problem_id);
		if(num != -1) values.add(num);
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
				throw new ServletException("Connection closing error",e);
			}
		}
		if(result.getRowCount() < 1) return -1;
		Map map = result.getRows()[0];
		int count = Integer.parseInt((map.get("count")).toString());
		return count;
	}
	*/
}
?>
<?php

class problem_solution_t
{
	//Property

	var $problem_id;
	var $start_row = 0;
	var $no_of_row;
	var $status;
	var $total_row;
	var $valid_array;
	var $order;
	var $isV_valid;
	var $problem_list = null;
	var $user_id;
	var $result_map = null;

	//construction
	function problem_solution_t($p_id){
		$this->problem_id = $p_id;
	}


	function set_problem_id($p_id){
		$this->problem_id = $problem_id;
	}

/*
	private void execute(Connection conn,String str)
		throws SQLException{
		ResultSet rs = null;
		Statement stmt = null;
		try{
			stmt = conn.createStatement();
			stmt.execute(str);
		}
		finally{
			if(rs != null){
				try{
					rs.close();
				}
				catch(SQLException e) {}
			}
			if(stmt != null){
				try{
					stmt.close();
				}
				catch(SQLException e){}
			}
		}
	}

	public void initialValidArray()
		throws SQLException,ServletException{
		int i;
		int count = Integer.parseInt(noOfRow);
		if(user_id == null || user_id.length() == 0){
			for(i = 0;i < count;i++) validArray[i] = false;
			return;
		}
		if(hasPrivilege(user_id)){
			for(i = 0;i < count;i++) validArray[i] = true;
			return;
		}
		if(resultMap == null){
			for(i = 0;i < count;i++) validArray[i] = false;
			return;
		}
		for(i = 0;i < resultMap.length;i++){
			String thatUser_id = (resultMap[i].get("user_id")).toString();
			String thatProblem_id = (resultMap[i].get("problem_id")).toString();
			validArray[i] = isValid(thatUser_id,thatProblem_id);
		}
	}

*/

	function is_valid(){

		return $this->valid_array[$this->order];
	}

	/*
	private boolean firstSearchUser = true;
	private boolean isValid(String thatUser_id,String thatProblem_id)
		throws SQLException,ServletException{
		if(thatUser_id != null && thatUser_id.length() != 0 &&
			thatUser_id.equals(user_id)) return true;
		if(firstSearchUser){
			firstSearchUser = false;
			getProblemList(user_id);
		}

		if(hadAC(thatProblem_id)) return true;
		else return false;
	}

	private void getProblemList(String user_id)
		throws SQLException,ServletException{
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();

		sql.append("select problem_id from solution where ")
			.append(" user_id = ? and result = 0 order by problem_id");

		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();
		values.add(user_id);
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
		if(result == null || result.getRowCount() < 1) return;
		problemList = new ArrayList();
		Map[] map = result.getRows();
		for(int i = 0;i < map.length;i++){
			String id = (map[i].get("problem_id")).toString();
			problemList.add(id);
		}

	}

	private boolean hadAC(String problem_id){
		if(problemList == null) return false;
		int low = 0,high = problemList.size() - 1;
		int mid,id = Integer.parseInt(problem_id);
		while(low <= high){
			mid = (low + high)/2;
			int listId = Integer.parseInt((String)problemList.get(mid));
			if(id == listId) return true;
			else if(id > listId) low = mid + 1;
			else high = mid - 1;
		}
		return false;
	}



	private boolean hasPrivilege(String user_id)
		throws SQLException,ServletException{
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);

		StringBuffer sql = new StringBuffer();
		sql.append("select rightstr,defunct from privilege where user_id = ? ");

		sqlCommandBean.setSqlValue(sql.toString());

		List values = new ArrayList();
		values.add(user_id);
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
		if(result.getRowCount() < 1) return false;
		Map[] maps = result.getRows();
		for(int i = 0;i < result.getRowCount();i++){
			if(maps[i].get("rightstr") != null && maps[i].get("rightstr").equals("viewsource")
				&&maps[i].get("defunct") != null && maps[i].get("defunct").equals("N"))
				return true;
		}
		return false;
	}
*/



	function get_status(){

		$row = array();
		$row['solution_id'] = '1111';
		$row['user_id'] = 'magiii';
		$row['memory'] = '1024';
		$row['time'] = '1024';
		$row['language'] ='g++';
		$row['code_length'] = '108';
		$row['in_date'] = '2007';
		$status = array();
		$status[0] = $row;
		return $status;
		/*
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);


		if(problem_id ==  null || problem_id.length() == 0){
			problem_id = new String("0");
		}

              	execute(conn,new String("drop view if exists soluview2"));
                execute(conn,new String("drop view if exists soluview"));
		execute(conn,new String("create view soluview as select * from solution where result = 0 and solution_id !=9329 and problem_id = \'"+problem_id
               +"\' order by time,memory"));
                execute(conn,new String("create view soluview2 as select * from soluview group by user_id "));

		StringBuffer sql = new StringBuffer();

		sql.append("select solution_id,user_id,problem_id,result,time,memory,language,code_length,in_date ")
			.append("from soluview2  order by time,memory ");
		sql.append(" limit "+startRow+","+noOfRow);

		sqlCommandBean.setSqlValue(sql.toString());
		List values = new ArrayList();


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
		if(result != null && result.getRowCount() >= 1){
			resultMap = result.getRows();
		}
		initialValidArray();

		return result;
		*/
	}


	function get_total_row(){

		return 1;
		/*
		Connection conn = dataSource.getConnection();

		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();

		sql.append("select count(distinct(user_id)) as count from solution where result = 0 ");
		//统计不同用户AC的个数
		if(problem_id == null || problem_id.length() == 0){
			problem_id = new String("0");
		}
		sql.append("and problem_id = ? ");




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
			try{ conn.close(); }
			catch(SQLException e) { }
		}

		if(result.getRowCount() < 1) return 0;
		Map map = result.getRows()[0];
		return Integer.parseInt((map.get("count")).toString());
		*/
	}
}

?>
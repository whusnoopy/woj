<?php

class user_status_t
{
	var $user_id = null;
	var $solved = null;
	var $submission = null;
	var $rank = null;
	var $solved_result  = null;
	var $exist = null;
	var $school = null;
	var $email = null;


	function user_status_t($user_id)
	{
		$this->user_id = $user_id;
		/*
		Connection conn = dataSource.getConnection();
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		sql.append("select solved, submit, school, email from user ").
			append("where user_id = ?");
		List values = new ArrayList();
		values.add(user_id);
		sqlCommandBean.setSqlValue(sql.toString());
		sqlCommandBean.setValues(values);
		Result userResult = null;
		try{
			userResult = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){ }
		finally{
			try{conn.close();}
			catch(SQLException e){ }
		}
		if (userResult.getRowCount() < 1)
		{
			userMap = null;
			return;
		}
		userMap = userResult.getRows()[0];
		*/

	}

	function exist()
	{
		/*
		Connection conn = dataSource.getConnection();
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		sql.append("select * from user ").
			append(" where user_id = ?");
		List values = new ArrayList();
		values.add(user_id);
		sqlCommandBean.setSqlValue(sql.toString());
		sqlCommandBean.setValues(values);
		Result temp3 = null;
		try{
			temp3 = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){}
		finally{
			try{conn.close();}
			catch(SQLException e) { }
		}
		if (temp3.getRowCount() < 1)
		{
			exist = new String("false");
			return exist;
		}
		else
		{
			exist = new String("true");
			return exist;
		}
		*/
		return true;
	}




	function get_solved()
	{
		return $this->solved;
	}

	function get_submission()
	{
		return $this->submission;
	}

	function get_school()
	{
		return $this->school;
	}

	function get_email()
	{
		return $this->email;
	}

	function get_rank()
	{
		/*
		Connection conn = dataSource.getConnection();
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		sql.append("select count(*) AC from user ").
			append(" where solved > ?");
		List values = new ArrayList();
		values.add(solved);
		sqlCommandBean.setSqlValue(sql.toString());
		sqlCommandBean.setValues(values);
		Result temp1 = null;
		try{
			temp1 = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){}
		finally{
			try{conn.close();}
			catch(SQLException e) { }
		}
		int ac = Integer.parseInt(temp1.getRows()[0].get("AC").toString());

		Connection conn1 = dataSource.getConnection();
		SQLCommandBean sqlCommandBeanAnother = new SQLCommandBean();
		sqlCommandBeanAnother.setConnection(conn1);
		StringBuffer sqlAnother = new StringBuffer();
		sqlAnother.append("select count(*)  SUB from user ").
			append(" where solved = ? AND submit < ?");
		List valuesAnother = new ArrayList();
		valuesAnother.add(solved);
		valuesAnother.add(submission);
		sqlCommandBeanAnother.setSqlValue(sqlAnother.toString());
		sqlCommandBeanAnother.setValues(valuesAnother);
		Result temp2 = null;
		try{
			temp2 = sqlCommandBeanAnother.executeQuery();
		}
		catch(SQLException e) { }
		finally{
			try{ conn1.close();}
			catch(SQLException e) { }
		}
		int sub = Integer.parseInt(temp2.getRows()[0].get("SUB").toString());



		rank = new Integer(ac + sub + 1).toString();
		//rank = new Integer(ac + 1).toString();
		*/
		$rank = 1;
		return $rank;
	}

	function get_solved_result()
	{
		/*
		Connection conn = dataSource.getConnection();
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		sql.append("select distinct(problem_id) from solution ").
			append(" where user_id = ? and result = 0 ").
			append(" order by problem_id");
		List values = new ArrayList();
		values.add(user_id);
		sqlCommandBean.setSqlValue(sql.toString());
		sqlCommandBean.setValues(values);
		solvedResult = null;
		try{
			solvedResult = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){ }
		finally{
			try{conn.close();}
			catch(SQLException e) { }
		}
		*/
		$solved_result = 1;
		return $solved_result;

	}
}

$us = new user_status_t("magiii");
echo $us->get_solved_result();
?>
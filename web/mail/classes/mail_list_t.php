<?php

class mail_list_t
{

	var $to_user = null;
	var $result = null;

	function mail_list_t($user)
	{
		$this->to_user = $user;
	}



	function get_result()
	{
		$row = array();
		$result = array();
		$row['mail_id'] = '0001';
		$row['from_user'] = 'mcje';
		$row['title'] = 'hello';
		$row['in_date'] = '2008-03-14';
		$result[] = $row;
		$row['mail_id'] = '0002';
		$row['from_user'] = 'liuqian';
		$row['title'] = 'how are you';
		$row['in_date'] = '2008-03-15';
		$result[] = $row;

		/*
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		Connection conn = dataSource.getConnection();
		sqlCommandBean.setConnection(conn);

		StringBuffer sql = new StringBuffer();
		sql.append(" select * from mail").
			append(" where to_user like '" + to_user + "'").
			append(" order by in_date DESC");
		sqlCommandBean.setSqlValue(sql.toString());

		result = null;
		try{
			result = sqlCommandBean.executeQuery();
		}
		catch(SQLException e){}
		finally{
			try{conn.close();}
			catch(SQLException e){}
		}
		*/

		return $result;
	}


}

?>
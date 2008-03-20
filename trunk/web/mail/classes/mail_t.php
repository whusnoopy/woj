<?php

//该类存储一个mail的详细信息

class mail_t
{

	var $mail_id = null;
	var $from_user = null;
	var $title = null;
	var $to_user = null;
	var $content = null;
	var $new_mail = null;
	var $reply = null;
	var $in_date = null;

	function mail_t($mail_id)
	{
		if ($mail_id == null)
		{
			return;
		}
		$this->mail_id = $mail_id;
		$this->from_user = 'mcje';
		$this->title = 'hello';
		$this->to_user = 'magiii';
		$this->content = 'hello, magiii';
		$this->new_mail = 1;
		$this->reply = 0;
		$this->in_date = '2007-03-14';

		/*
		SQLCommandBean sqlCommandBean = new SQLCommandBean();
		Connection conn = dataSource.getConnection();
		sqlCommandBean.setConnection(conn);
		StringBuffer sql = new StringBuffer();
		sql.append("select * from mail").
			append(" where mail_id =" + mail_id);
		sqlCommandBean.setSqlValue(sql.toString());
		Result result = sqlCommandBean.executeQuery();
		if(result.getRowCount() < 1) sortedMap = null;
		else sortedMap = result.getRows()[0];

		//update the message's new_mail fragment
		StringBuffer sqlUpdate = new StringBuffer();
		sqlUpdate.append("update mail set new_mail = 0 where mail_id =" + mail_id);
		sqlCommandBean.setSqlValue(sqlUpdate.toString());
		try{
			sqlCommandBean.executeUpdate();
		}
		catch(SQLException e){ }
		finally{
			try{conn.close();}
			catch(SQLException e){}
		}
		*/


	}

}

?>
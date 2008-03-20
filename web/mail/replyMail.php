<?php

	include('../include/header.php');
	include('classes/mail_t.php');
	if (isset($_GET['mail_id']))
		$mail_id = $_GET['mail_id'];
	else
		$mail_id = '';
	$mail = new mail_t($mail_id);

/*
<jsp:useBean id="validUser" class="com.whu.noah.beans.user.UserBean" scope="session" />
<jsp:useBean id="mailValidateBean" class="com.whu.noah.beans.validate.mail.MailValidateBean" scope="page" />

<jsp:setProperty name="mailValidateBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="mailValidateBean" property="user_id" value="${validUser.uid}" />
<jsp:setProperty name="mailValidateBean" property="mail_id" value="${param.mail_id}" />

<c:if test="${fn:contains(mailValidateBean.authenticate, 'false')}">
  <jsp:forward page="mailerror.jsp">
    <jsp:param name="errorMsg" value="${validUser.uid}" />
  </jsp:forward>
</c:if>

<jsp:useBean id="mailBean" class="com.whu.noah.beans.mail.MailBean" scope="page" />
<jsp:setProperty name="mailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="mailBean" property="mail_id" value="${param.mail_id}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />
*/
?>

  <div id="tt">Reply Mail</div>

<?php
	include('../include/notice.php');
?>

  <div id="main">
    <form action="mailAction.do.php" method=post>
	  <input type="hidden" name="from_user" value="<?php echo $mail->from_user; ?>" />
	  <input type="hidden" name="type" value="reply" />
	  <table><tbody>
<tr class=tro>
  <td width=80></td>
  <td width=100 align=right><strong>To:&nbsp;&nbsp;</strong></td>
  <td width=700 align=left>&nbsp;&nbsp;<input size=15 name="to_user" value="<?php echo $mail->from_user; ?>" /> (Write Receiver's User Id here) </td>
  <td width=70></td>
</tr>
<tr class=tre>
  <td></td>
  <td align=right><strong>Title:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<input size=80 name="title" value="Re:<?php echo $mail->title; ?>" /></td>
  <td></td>
</tr>
<tr class=tro>
  <td></td>
  <td align=right valign=top><strong>Content:&nbsp;&nbsp;</strong></td>
  <td align=left>&nbsp;&nbsp;<textarea name="content" rows=20 cols=80></textarea></td>
  <td></td>
</tr>
<tr class=tre>
  <td align=center colspan=4><input type=submit value=Send name=b1 /></td>
</tr>
      </tbody></table>
    </form>
  </div>

<?php
	include('../include/tailer.php');
?>

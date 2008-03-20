
<?php
	include('..\include\header.php');
	include('classes\mail_t.php');
	if (isset($_GET['mail_id']))
		$mail_id = $_GET['mail_id'];
	else
		$mail_id = -1;

	$mail = new mail_t($mail_id);

/*
<jsp:useBean id="mailBean" class="com.whu.noah.beans.mail.MailBean" scope="page" />
<jsp:useBean id="formatBean" class="com.whu.noah.beans.admin.FormatOutputBean" scope="page" />
<jsp:useBean id="validUser" class="com.whu.noah.beans.user.UserBean" scope="session" />
<jsp:useBean id="mailValidateBean" class="com.whu.noah.beans.validate.mail.MailValidateBean" scope="page" />

<jsp:setProperty name="mailValidateBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="mailValidateBean" property="user_id" value="${validUser.uid}" />
<jsp:setProperty name="mailValidateBean" property="mail_id" value="${param.mail_id}" />

<c:if test="${fn:contains(mailValidateBean.authenticate, 'false')}">
  <jsp:forward page="mailerror.jsp">
    <jsp:param name="errorMsg" value="mail doesn't exist" />
  </jsp:forward>
</c:if>

<jsp:setProperty name="mailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="mailBean" property="mail_id" value="${param.mail_id}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />
*/
?>


  <div id="tt">Mail</div>
<?php
	include('.\include\notice.php');
?>
  <div id="main">

  <table><tbody>
  <tr>
    <th width=80></th>
    <th width=100 align=right>Title:&nbsp;&nbsp;</th>
    <th width=700 align=left><?php echo $mail->title; ?></th>
    <th width=70></th>
  </tr>
  <tr class=tro>
    <td></td>
    <td align=right><strong>From:&nbsp;&nbsp;</strong></td>
    <td align=left><?php echo $mail->from_user; ?></td>
    <td></td>
  </tr>
  <tr class=tre>
    <td></td>
    <td align=right><strong>Send Time:&nbsp;&nbsp;</strong></td>
    <td align=left><?php echo $mail->in_date; ?></td>
    <td></td>
  </tr>
  <tr class=tro valign=top>
    <td></td>
    <td align=right><strong>Content:&nbsp&nbsp;</strong></td>
    <td align=left><textArea name="content" rows=20 cols=80><?php echo $mail->content; ?></textArea></td>
    <td></td>
  </tr>
  </tbody></table>

  <br />
  <span class="bt"><a href="mailList.php">Return</a></span>&nbsp;&nbsp;
  <span class="bt"><a href="replyMail.php?mail_id=<?php echo $mail_id; ?>&type=reply">Reply</a></span>&nbsp;&nbsp;
  <span class="bt"><a href="mailAction.do.php?mail_id=<?php echo $mail_id; ?>&type=delete">Delete</a></span>
  <br /><br />
  </div>

<?php
	include('..\include\tailer.php');
?>

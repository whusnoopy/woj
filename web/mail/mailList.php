<?php
	include ('../include/header.php');
	include('classes/mail_list_t.php');

?>

<?php
	$no_of_row = 20;
	if(!$user_id){
		header("Location: ../user/login.php?origURL=../mail/mailList.php");
		exit;
	}

	if(isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = 0;

	$current = $start;
	$mail_list = new mail_list_t($user_id);
	$list = $mail_list->get_result();


/*
<c:set var="noOfRows" value="20" scope="page" />

<jsp:useBean id="validUser" class="com.whu.noah.beans.user.UserBean" scope="session" />
<c:if test="${validUser.uid == null}">
  <c:redirect url="../user/login.jsp">
    <c:param name="origURL" value="../mail/mailList.jsp" />
  </c:redirect>
</c:if>

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="mailListBean" class="com.whu.noah.beans.mail.MailListBean" scope="page" />
<jsp:setProperty name="mailListBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="mailListBean" property="to_user" value="${user_id}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />

<c:choose>
  <c:when test="${param.start == null}">
    <c:set var="current" value="0" scope="page" />
  </c:when>
  <c:otherwise>
    <c:set var="current" value="${param.start}" scope="page" />
  </c:otherwise>
</c:choose>
*/
?>




  <div id="tt"><?php echo $user_id; ?>'s mailbox</div>

<?php
	include('../include/notice.php');
?>
  <div id="main">
    <table><tbody>
  <tr>
    <th width=80>No.</td>
    <th width=170>From</td>
    <th width=500>Title</td>
    <th width=200>Date</th>
  </tr>

<?php
  $lineColor = 0;
  foreach ($list as $row){

	$current++;
    if($lineColor == 0){
      echo '<tr class=tro>';
      $lineColor = 1;
	}
	else{
	  echo '<tr class=tre>';
      $lineColor = 0;
	}

	echo "<td><a href=mail.php?mail_id=$row[mail_id]>$current</a></td>";
    echo "<td><a href=../status/userStatus.php?user_id=$row[from_user]>$row[from_user]</a></td>";
    if($row['new_mail'] == 1)
      echo '<td bgcolor=#ffff7f>';
    else
      echo '<td>';
    echo "<a href=mail.php?mail_id=$row[mail_id]>$row[title]</a>";
    echo '</td>';
	echo "<td>$row[in_date]</td>";
    echo '</tr>';
  }
?>
    </tbody></table>
    <br />
<?php
    if ($start - $no_of_row > 0)
		echo "<span class=bt><a href=mailList.php?start=$start - $no_of_row>Previous</a></span>&nbsp;&nbsp;";

    if ($start + $no_of_row < count($list))
		echo "<span class=bt><a href=mailList.php?start=$start + no_of_row}>Next</a></span>&nbsp;&nbsp;";

?>
	<span class=bt><a href="sendMail.php">Send mail</a></span>
    <br /><br />
  </div>

<?php
	include('../include/tailer.php');
?>

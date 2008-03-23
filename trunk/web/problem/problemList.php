<?php
/*
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html xmlns="http://www.w3.org/1999/xhtml">

<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%@ taglib prefix="sql" uri="http://java.sun.com/jsp/jstl/sql" %>

<c:set var="noOfRows" value="100" scope="page" />

<jsp:useBean id="validUser" class="com.whu.noah.beans.user.UserBean" scope="session" />
<jsp:useBean id="checkSolvedBean" class="com.whu.noah.beans.problem.CheckSolvedBean" scope="page" />
<jsp:setProperty name="checkSolvedBean" property="dataSource" value="${appDataSource}" />
<jsp:useBean id="ratioChangeBean" class="com.whu.noah.util.RatioChangeBean" scope="page" />
<jsp:useBean id="problemListBean" class="com.whu.noah.beans.problem.ProblemListBean" scope="page" />
<jsp:setProperty name="problemListBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="problemListBean" property="start" value="${param.start}" />
<jsp:setProperty name="problemListBean" property="num" value="${noOfRows}" />
<c:if test="${param.type != null}">
  <jsp:setProperty name="problemListBean" property="type" value="${param.type}" />
</c:if>
<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />

<jsp:setProperty name="checkSolvedBean" property="user_id" value="${validUser.uid}" />
<jsp:setProperty name="checkSolvedBean" property="current" value="0" />
*/?>

<?php
	include('../include/header.php');
	if (isset($_GET['volume']))
		$volume = intval( $_GET['volume'] );
	else
		$volume = 1;
	if (isset($_GET['start']))
		$start = intval($_GET['start']);
	else $start = 1;

	$noOfRows = 100;
?>

  <div id="tt">Problems Volume <?php echo $volume; ?></div>

<?php
	include('../include/notice.php');
?>




  <div id="main">
  <table><tbody align=center>
    <tr>
      <th width="80">&nbsp;</th>
      <th width="100"><a href="problemList.php?type=id&start=<?php echo $start; ?>&volume=<?php echo $volume;?>">ID</a></th>
      <th width="590">Title</th>
      <th width="180"><a href="problemList.php?type=Ratio&start=<?php echo $start; ?>&volume=<?php echo $volume;?>">Ratio</a>&nbsp;(<a href="problemList.php?type=accepted&start=<?php echo $start; ?>&volume=<?php echo $volume;?>">AC</a>/<a href="problemList.php?type=submit&start=<?php echo $start; ?>&volume=<?php echo $volume;?>">Total</a>)</th>
    </tr>
<?php
/*
    <c:set var="lineColor" value="0" />
    <c:forEach items="${problemListBean.result.rows}" var="row">
    <jsp:setProperty name="ratioChangeBean" property="input" value="${row.ratio}" />
    <c:choose>
      <c:when test="${lineColor == 0}">
        <tr class=tro>
        <c:set var="lineColor" value="1" />
      </c:when>
      <c:otherwise>
        <tr class=tre>
        <c:set var="lineColor" value="0" />
      </c:otherwise>
    </c:choose>
      <td>
        <c:if test="${!(validUser.uid == null)}">
          <jsp:setProperty name="checkSolvedBean" property="problem_id" value="${row.problem_id}" />
          <c:choose>
            <c:when test="${checkSolvedBean.solved == 2}">
              <img src="../image/accepted.gif" />
            </c:when>
            <c:otherwise>
              <c:if test="${checkSolvedBean.solved == 1}">
                <img src="../image/wrong.gif" />
              </c:if>
			</c:otherwise>
          </c:choose>
        </c:if>
      </td>
      <td>${fn:escapeXml(row.problem_id)}</td>
      <td><a href="problem.php?problem_id=${row.problem_id}">${row.title}</a></td>
      <td>${ratioChangeBean.output}%&nbsp;(${row.accepted}/${row.submit})</td>
    </tr>
    </c:forEach>
*/
?>
<tr class=tro>
<td> <img src="../image/accepted.gif" /></td>
<td>1001</td>
<td><a href="problem.php?problem_id=1001">a+b</a></td>
<td>66%&nbsp;(20/30)</td>
</tr>


  </tbody></table>
  <br>
  <span class="bt"><a href="problemSet.php">Back to Volumes List</a></span>
  <br><br>
  </div>

<?php
	include('../include/tailer.php');
?>

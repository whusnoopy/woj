
<?php
/*
<jsp:useBean id="validUser" class="com.whu.noah.beans.user.UserBean" scope="session" />
<jsp:useBean id="ratioChangeBean" class="com.whu.noah.util.RatioChangeBean" scope="page" />
<jsp:useBean id="searchBean" class="com.whu.noah.beans.problem.SearchBean" scope="page" />
<jsp:useBean id="checkSolvedBean" class="com.whu.noah.beans.problem.CheckSolvedBean" scope="page" />
<jsp:setProperty name="checkSolvedBean" property="dataSource" value="${appDataSource}" />

<jsp:setProperty name="searchBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="searchBean" property="key" value="${param.key}" />
<jsp:setProperty name="searchBean" property="field" value="${param.field}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />

<jsp:setProperty name="checkSolvedBean" property="user_id" value="${validUser.uid}" />
<jsp:setProperty name="checkSolvedBean" property="current" value="0" />
*/
?>

<?php
	$key = $_POST['key'];
	$field = $_POST['field'];
?>

<?php
	include('..\include\header.php');
?>
  <div id="tt">
    Problem Search Result - <?php echo $key.' - '.$field;?>
  </div>
<?php
	include ('..\include\notice.php');
?>

  <div id="main">

    <table><tbody>
    <tr>
      <th width="50">&nbsp;</th>
      <th width="80">ID</a></th>
      <th width="300">Title</th>
      <th width="370">Source</th>
      <th width="150">Ratio&nbsp;(AC/Total)</th>
    </tr>

<?php /*
    <c:set var="lineColor" value="0" />
    <c:forEach items="${searchBean.result.rows}" var="row">
    <jsp:setProperty name="ratioChangeBean" property="input" value="${row.ratio}" />
    <c:choose>
      <c:when test="${lineColor == 0}">
        <tr class="tro">
        <c:set var="lineColor" value="1" />
      </c:when>
      <c:otherwise>
        <tr class="tre">
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
      <td><a href="problem.jsp?problem_id=${row.problem_id}">${row.title}</a></td>
      <td>${row.source}</td>
      <td>${ratioChangeBean.output}%&nbsp;(${row.accepted}/${row.submit})</td>
    </tr>
    </c:forEach>
*/?>

  </tbody></table>
<?php /*
<br>
  <span style="font-size:16px">In all, there
  <c:choose>
    <c:when test="${searchBean.result.rowCount == null}">is <font color="red"><strong>no</strong></font> problem</c:when>
    <c:otherwise>
      <c:choose>
        <c:when test="${searchBean.result.rowCount == 0}">is <font color="red"><strong>no</strong></font> problem</c:when>
        <c:when test="${searchBean.result.rowCount == 1}">is <font color="green"><strong>1</strong></font> problem</c:when>
        <c:otherwise>are <font color="green"><strong><c:out value="${searchBean.result.rowCount}" /></strong></font> problems</c:otherwise>
      </c:choose>
    </c:otherwise>
  </c:choose>
  match.</span>
*/?>
  <br><br>
  <span class="bt"><a href="problemSet.php">Back to Volumes List</a></span>
  <br><br>

  </div>


<?php
    include ('..\include\tailer.php');
?>

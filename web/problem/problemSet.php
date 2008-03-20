
<?php
/*
<c:if test="${problemList == null}">
  <sql:query var="problemList" scope="request" dataSource="${appDataSource}" sql="SELECT * FROM problem ORDER BY problem_id"/>
</c:if>
<c:set var="num" value="${problemList.rowCount}" />
<c:set var="noOfRows" value="100" />
<c:set var="pageNum" value="${(num + noOfRows - 1) / noOfRows}" />

<jsp:useBean id="numberChangeBean" class="com.whu.noah.util.NumberChangeBean" scope="page" />
<jsp:useBean id="maxProblemBean" class="com.whu.noah.beans.problem.MaxProblemBean" scope="page" />
<jsp:setProperty name="maxProblemBean" property="dataSource" value="${appDataSource}" />
*/


?>

<?php
	include('..\include\header.php');
?>

  <div id="tt">
    Problem Set
  </div>

<?php
	include ('..\include\notice.php');
?>

  <div id="main">

  <table><tbody>
  <tr>
    <th width=200></th>
    <th width=100>From</th>
    <th width=350>Problems Volume</th>
    <th width=100>To</th>
    <th width=200></th>
  </tr>

  <tr class="tro">
  <td></td>
  <td align="center">1001</td>
  <td align="center"><a href="problemList.php?start=1;volume=1" class="blue">Volume 1</a></td>
  <td align="center">1100</td>
  <td></td>
  </tr>

<?php
/*
    <c:set var="lineColor" value="0" />
    <c:forEach begin="1" end="${pageNum}" var="current">
      <jsp:setProperty name="numberChangeBean" property="number" value="${current}" />
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
      <td></td>
      <c:choose>
        <c:when test="${(current - 1) * 100 + 1001 <= maxProblemBean.maxProblem_id}">
		<td align="center">${(current - 1) * 100 + 1001}</td>
		<td align="center"><a href="problemList.jsp?start=${(current - 1) * noOfRows + 1}&amp;volume=${numberChangeBean.roman}" class="blue">Volume ${numberChangeBean.roman}</a></td>
		<td align="center">
          <c:choose>
            <c:when test="${current * 100 <= num }">
              <c:choose>
                <c:when test="${current * 100 + 1000 <= maxProblemBean.maxProblem_id}">
                  ${current * 100 + 1000}
                </c:when>
                <c:otherwise>
                  ${maxProblemBean.maxProblem_id}
                </c:otherwise>
              </c:choose>
            </c:when>
            <c:otherwise>
              ${maxProblemBean.maxProblem_id}
            </c:otherwise>
		  </c:choose>
        </td>
        <td></td>
      </tr>
        </c:when>
      </c:choose>
	</c:forEach>
*/?>
  </tbody></table>

  <br />


  <div>
    <form method=post action="searchList.php">
	<strong>Search:</strong>&nbsp;<input name=key type=text value='' size=50 maxlength="255" />&nbsp;
    <strong>By:</strong>&nbsp;<select name=field><option value="title">Title</option><option value="source">Source</option></select>&nbsp;
    <input type=submit value=GO />
    </form>
  </div>
  </div>

<?php
	include('..\include\tailer.php');
?>

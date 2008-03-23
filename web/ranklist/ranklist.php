<?php
    if(isset($_GET['start']))
		$ranknum = $_GET['start'];
	else
		$ranknum = '0';
?>

<?php
  include('../include/header.php');
?>
  <div id="tt">User Ranklist</div>
<?php
  include('../include/notice.php');
?>

  <div id="main">

<table><tbody>
  <tr>
    <th width="80">No.</th>
    <th width="120">User ID</th>
    <th width="450">Nick Name</th>
    <th width="100"><a href="ranklist.php">Solved</a></th>
    <th width="100"><a href="ranklist.php?of1=submit">Submit</a></th>
    <th width="100"><a href="ranklist.php?of1=ratio">Ratio</a></th>
  </tr>
 <%--
  <c:set var="lineColor" value="0" />
  <c:forEach items="${ranklist.rows}" var="row" begin="0" end="${noOfRows - 1}">
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
    <c:set var="ranknum" value="${ranknum + 1}" />
    <td height="26">${fn:escapeXml(ranknum)}</td>
    <td><a href="../status/userStatus.jsp?user_id=${row.user_id}">${fn:escapeXml(row.user_id)}</a></td>
    <td>
     <c:choose>
     <c:when test="${fn:contains(row.nick,'<')}">${fn:escapeXml(row.nick)}</c:when>
     <c:otherwise>${row.nick}</c:otherwise>
     </c:choose>
    </td>
    <td>${fn:escapeXml(row.solved)}</td>
    <td>${fn:escapeXml(row.submit)}</td>
    <td>${ratioChangeBean.output}%</td>
    </tr>
  </c:forEach>
 --%>
</tbody></table>

<%--
  <br><div>
   <c:choose>
    <c:when test="${param.start > 0}">
     <span class="bt">
     <c:if test="${empty param.of1}">
      <a href="ranklist.jsp?start=${param.start - noOfRows}">
     </c:if>
     <c:if test="${!empty param.of1}">
      <a href="ranklist.jsp?start=${param.start - noOfRows}&of1=${param.of1}">
     </c:if>
     &nbsp;Prev Page&nbsp;</a></span>
    </c:when>
  </c:choose>
  &nbsp;
  <c:choose>
   <c:when test="${param.start + noOfRows  < dupUserList.rowCount}">
    <span class="bt">
    <c:if test="${empty param.of1}">
     <a href="ranklist.jsp?start=${param.start + noOfRows}">
    </c:if>
    <c:if test="${!empty param.of1}">
     <a href="ranklist.jsp?start=${param.start + noOfRows}&of1=${param.of1}">
    </c:if>
     &nbsp;Next Page&nbsp;</a></span>
   </c:when>
  </c:choose>
  </div><br>
  </div>
 --%>

<?php
	include('../include/tailer.php');
?>


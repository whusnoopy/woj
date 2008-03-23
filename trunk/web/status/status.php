
<?php
	include('../include/header.php');
    echo '<div id="tt">Status</div>';
	include('../include/notice.php');
?>

  <div id="main">
<?php
/*
  <c:set var="noOfRows" value="20" />
  <jsp:useBean id="statusBean" class="com.whu.noah.beans.status.StatusBean" scope="page"/>
  <jsp:useBean id="statusDisplayBean" class="com.whu.noah.beans.status.StatusDisplayBean" scope="page"/>
  <jsp:setProperty name="statusBean" property="problem_id" value="${param.problem_id}" />
  <jsp:setProperty name="statusBean" property="user_id" value="${param.user_id}" />
  <jsp:setProperty name="statusBean" property="result" value="${param.result}" />
  <jsp:setProperty name="statusBean" property="language" value="${param.language}" />
  <jsp:setProperty name="statusBean" property="contest_id" value="${param.contest_id}" />
  <jsp:setProperty name="statusBean" property="dataSource" value="${applicationScope.appDataSource}" />
  <jsp:setProperty name="statusBean" property="startRow" value="${param.start}" />
  <jsp:setProperty name="statusBean" property="noOfRow" value="${noOfRows}" />

  <c:set value="${statusBean.status}"  var = "status" scope = "page" />
  <c:set value="${statusBean.totalRow}" var = "totalRow" scope = "page" />

  <jsp:useBean id="accessJudgeBean1" class = "com.whu.noah.beans.source.AccessJudgeBean1" scope = "page"/>
  <jsp:setProperty name="accessJudgeBean1" property="dataSource" value="${applicationScope.appDataSource}" />
  <jsp:setProperty name="accessJudgeBean1" property="user_id" value="${validUser.uid}" />
  <jsp:setProperty name="accessJudgeBean1" property="start" value="${param.start}" />
  <jsp:setProperty name="accessJudgeBean1" property="noOfRow" value="${noOfRows}" />
  <jsp:setProperty name="accessJudgeBean1" property="searchUser_id" value="${param.user_id}" />
  <jsp:setProperty name="accessJudgeBean1" property="problem_id" value="${param.problem_id}" />
  <jsp:setProperty name="accessJudgeBean1" property="result" value="${param.result}" />
  <jsp:setProperty name="accessJudgeBean1" property="language" value="${param.language}" />
  <jsp:setProperty name="accessJudgeBean1" property="contest_id" value="${param.contest_id}" />
*/
	$noOfRows = 20;


?>

  <table><tbody>

  <tr>
    <th width="70">Run ID</th>
    <th width="155">User</th>
    <th width="75">Problem</th>
    <th width="200">Result</th>
    <th width="70">Memory</th>
    <th width="60">Time</th>
    <th width="85">Language</th>
    <th width="75">Length</th>
    <th width="160">Submit Time</th>
  </tr>
<?php
/*
  <c:set var="lineColor" value="0" />
  <c:forEach items="${status.rows}" var="row" begin="0" end="${noOfRows - 1}">
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
    <td>${fn:escapeXml(row.solution_id)}</td>
    <td><a href="userStatus.jsp?user_id=${row.user_id}">${fn:escapeXml(row.user_id)}</a></td>
    <td><a href="../problem/problem.jsp?problem_id=${row.problem_id}">${fn:escapeXml(row.problem_id)}</a></td>
    <td>
     <jsp:setProperty name="statusDisplayBean" property="intResult"	value="${row.result}" />
     <c:choose>
      <c:when test="${fn:contains(statusDisplayBean.strResult, 'Runtime Error(JAVA)')}">
       <a href="javare.jsp?solution_id=${row.solution_id}" target="_blank"><font size="2" color="blue">${fn:escapeXml(statusDisplayBean.strResult)}</font></a>
      </c:when>
      <c:when test="${fn:contains(statusDisplayBean.strResult, 'Compile Error')}">
       <a href="compileerror.jsp?solution_id=${row.solution_id}" target="_blank"><font size="2" color="blue">${fn:escapeXml(statusDisplayBean.strResult)}</font></a>
      </c:when>
      <c:when test="${fn:contains(statusDisplayBean.strResult, 'Accepted')}">
       <span class="STYLE7"><font color="red">${fn:escapeXml(statusDisplayBean.strResult)}</font></span>
      </c:when>
      <c:when test="${fn:contains(statusDisplayBean.strResult, 'Runtime Error(Pascal)')}">
       <a href="pascalre.jsp?solution_id=${row.solution_id}" target="_blank"><font size="2" color="blue">${fn:escapeXml(statusDisplayBean.strResult)}</font></a>
      </c:when>
      <c:otherwise>
       <span class="STYLE7"><font color="green">${fn:escapeXml(statusDisplayBean.strResult)}</font></span>
      </c:otherwise>
     </c:choose>
    </td>
	<td>${fn:escapeXml(row.memory)}</td>
    <td>${fn:escapeXml(row.time)}</td>
    <jsp:setProperty name="statusDisplayBean" property="intLanguage" value="${row.language}" />
    <td>
     <!--Authenticate whether the access to source is valid	-->
     <jsp:setProperty name="accessJudgeBean1" property="order" value="${order}" />
     <c:set var="order" value="${order+1}" />
     <c:set var="isValid" value="${accessJudgeBean1.isValid}" scope="page" />
     <c:choose>
      <c:when test="${isValid}">
       <a href="../source/source.jsp?solution_id=${row.solution_id}" target="_blank">${fn:escapeXml(statusDisplayBean.strLanguage)}</a>
      </c:when>
      <c:otherwise>${fn:escapeXml(statusDisplayBean.strLanguage)}</c:otherwise>
     </c:choose>
    </td>
	<td>${fn:escapeXml(row.code_length)}</td>
    <td>${fn:escapeXml(row.in_date)}</td>
   </tr>
  </c:forEach>
*/
?>
  </tbody></table>

  <br />
  <form action="status.php" method="get">
   <input type="hidden" name="contest_id" value="${param.contest_id}" />
   <strong>Select status by</strong>&nbsp;
   <strong>Problem ID:</strong> <input size=8 name="problem_id" />&nbsp;
   <strong>User ID:</strong> <input size=15 name="user_id" />&nbsp;
   <strong>Result:</strong> <select size=1 name="result" />
    <option value="" selected>All</option>
    <option value=0>Accepted</option>
    <option value=1>Presentation Error</option>
    <option value=2>Time Limit Exceed</option>
    <option value=3>Memory Limit Exceed</option>
    <option value=4>Wrong Answer</option>
    <option value=5>Output Limit Exceed</option>
    <option value=6>Compile Error</option>
    <option value=7>Runtime Error(SIGSEGV)</option>
    <option value=8>Runtime Error(SIGFPE)</option>
    <option value=9>Runtime Error(SIGBUS)</option>
    <option value=10>Runtime Error(SIGABRT)</option>
    <option value=11>Runtime Error(JAVA)</option>
    <option value=12>Runtime Error(Pascal)</option>
    <option value=13>Restricted Function</option>
    <option value=14>System Error</option>
   </select>&nbsp;
   <strong>Language:</strong> <select size=1 name="language" >
    <option value="" selected>All</option>
    <option value=0>GCC</option>
    <option value=1>G++</option>
    <option value=2>Java</option>
    <option value=3>Pascal</option>
   </select>&nbsp;
   <%-- Last: <input size=15 name="start" />&nbsp; --%>
   <input type=submit value="Go" width="8" />
  </form>
  <br />
  <div>
    <c:choose>
      <c:when test="${param.start > 0}">
       <span class="bt"><a href="status.jsp?start=0&contest_id=${param.contest_id}&problem_id=${param.problem_id}&result=${param.result}&user_id=${param.user_id}&language=${param.language}">Top</a></span>&nbsp;
       <span class="bt"><a href="status.jsp?start=${param.start - noOfRows}&contest_id=${param.contest_id}&problem_id=${param.problem_id}&result=${param.result}&user_id=${param.user_id}&language=${param.language}">Prev Page</a></span>&nbsp;
      </c:when>
    </c:choose>
    <c:choose>
      <c:when test="${param.start + noOfRows  < totalRow}">
       <span class="bt"><a href="status.jsp?start=${param.start + noOfRows}&contest_id=${param.contest_id}&problem_id=${param.problem_id}&result=${param.result}&user_id=${param.user_id}&language=${param.language}">Next Page</a></span>
      </c:when>
    </c:choose>
  </div>
  <br />
  </div>


<?php
	include('..\include\tailer.php');
?>

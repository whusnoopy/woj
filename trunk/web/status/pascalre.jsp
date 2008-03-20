<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<%@ page contentType="text/html" %>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>

<jsp:useBean id="pascalREBean" class="com.whu.noah.beans.status.PascalREBean" scope="page" />
<jsp:setProperty name="pascalREBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="pascalREBean" property="solution_id" value="${param.solution_id}" />

<head>
  <meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
  <title>Pascal Runtime Error</title>
  <link href="../style/noah.css" rel="stylesheet" type="text/css" />
</head>

<body>
<center>
  <%-- head bar of Page --%>
  <div id="bar">
    <a href="../home.jsp">Home</a>&nbsp;|&nbsp;
    <c:choose>
	  <c:when test="${validUser.uid == null}">
		<a href="../user/login.jsp">Login</a>
    </c:when>
      <c:otherwise>
	    <a href="../user/logout.do">Logout</a>
      </c:otherwise>
    </c:choose>&nbsp;|&nbsp;
    <a href="../problem/problemSet.jsp">Problems</a>&nbsp;|&nbsp;
    <a href="../contest/contestList.jsp">Contests</a>&nbsp;|&nbsp;
    <a href="../submit/submit.jsp">Submit</a>&nbsp;|&nbsp;
    <a href="../status/status.jsp">Status</a>&nbsp;|&nbsp;
    <a href="../ranklist/ranklist.jsp">Ranklist</a>&nbsp;|&nbsp;
    <a href="../user/register.jsp">Register</a>&nbsp;|&nbsp;
    <a href="../user/modify.jsp">Setting</a>&nbsp;|&nbsp;
    <a href="../mail/mailList.jsp">
      <c:choose>
        <c:when test="${hasNewMail==true}">
          <font color="red">Mail</font>
        </c:when>
        <c:otherwise>Mail</c:otherwise>
      </c:choose>
    </a>&nbsp;|&nbsp;
    <a href="../../forum" target="_blank">Forum</a>&nbsp;|&nbsp;
    <a href="../faq.html" target="_blank">FAQ</a>
  </div>

  <div id="main">
   <br />
   <div>
     <span class="cl">Pascal Runtime Error</span>
   </div>
   <br />
   <div class="ptt">Information</div>
   <div class="code"><pre>${pascalREBean.error}</pre></div>
  </div>
  
  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>
 
 </center>
</body>
</html>


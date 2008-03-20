<?php
/*
<jsp:useBean id="problemBean" class="com.whu.noah.beans.problem.ProblemBean" scope="page" />
<jsp:useBean id="format" class="com.whu.noah.beans.admin.FormatOutputBean" scope="page" />
<jsp:useBean id="dateBean" class="com.whu.noah.beans.admin.DateBean" scope="page" />

<jsp:setProperty name="problemBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="problemBean" property="problem_id" value="${param.problem_id}" />

<c:set var="user_id" value="${validUser.uid}" scope="page" />
<jsp:useBean id="checkNewMailBean" class="com.whu.noah.beans.mail.CheckNewMailBean" scope="page" />
<jsp:setProperty name="checkNewMailBean" property="dataSource" value="${appDataSource}" />
<jsp:setProperty name="checkNewMailBean" property="user_id" value="${user_id}" />
<c:set var="hasNewMail" value="${checkNewMailBean.newMail}" />
*/

	if (isset($_GET['problem_id']))
		$problem_id = intval( $_GET['problem_id'] );
	else
		$problem_id = 0;
	$problem_title = "a + b";
	$time_limit = 1000;
	$memory_limit = 1024;
	$submit = 60;
	$accepted = 40;
	$description = "a + b";
	$input = "a, b";
	$output = "a+b";
	$sample_input = "1 2";
	$sample_output = "3";
	$hint = null;
?>

<?php
	include('..\include\header.php');
?>

  <div id="tt">
    <?php echo "Problem ".$problem_id.' - '.$problem_title;?>
  </div>

<?php
	include('..\include\notice.php');
?>

  <div class="ifm">
    <strong>Time Limit</strong>:${problemBean.time_limit}MS&nbsp;<strong>Memory Limit</strong>:${problemBean.memory_limit}KB<br>
    <strong>Total Submit</strong>:${problemBean.submit}&nbsp;<strong>Accepted</strong>:${problemBean.accepted}<br>
  </div>

  <div id="main">
    <div class="ptt">Description</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.description}" />
    <div class="ptx">${format.output}</div>

    <div class="ptt">Input</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.input}" />
    <div class="ptx">${format.output}</div>

    <div class="ptt">Output</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.output}" />
    <div class="ptx">${format.output}</div>

    <div class="ptt">Sample Input</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.sample_input}" />
    <div class="code">${format.output}</div>

    <div class="ptt">Sample Output</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.sample_output}" />
    <div class="code">${format.output}</div>

    <div class="ptt">Hint</div>
    <jsp:setProperty name="format" property="input" value="${problemBean.hint}" />
    <div class="ptx">${format.output}</div>

    <div class="ptt">Source</div>

    <jsp:setProperty name="dateBean" property="start_time" value="${problemBean.start_time}" />
    <jsp:setProperty name="dateBean" property="end_time" value="${problemBean.end_time}" />
    <jsp:setProperty name="format" property="input" value="${problemBean.source}" />

    <div class="ptx">
    <c:if test="${fn:contains(dateBean.startTest, 'beforeCurrent')}">
      <c:if test="${fn:contains(dateBean.endTest, 'beforeCurrent')}">
		<div>${format.output}</div>
	  </c:if>
	</c:if>
    </div>
    <br />
    <div>
      <span class="bt"><a href="../submit/submit.php?problem_id=<?php echo $problem_id;?>">Submit</a></span>&nbsp;&nbsp;
      <span class="bt"><a href="../status/problemstatus.php?problem_id=<?php echo $problem_id;?>">Status</a></span>
    </div>
    <br />
  </div>

<?php
	include('..\include\tailer.php');
?>
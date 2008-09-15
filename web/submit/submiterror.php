<?php
session_start();
include ('../include/header.php');
echo '<title>Error</title>';
?>
  <div id="tt">Submit</div>

<?php
	$error = $_GET['error'];
	include('../include/notice.php');
?>
  <div id="main">
    <div><span class="cl">Error occured during your submit</span></div>
    <br />
    <div class="ptt">Information</div>
    <div class="ptx"><pre><?php echo $error;?></pre></div>
  </div>
<?php
  include("../include/tailer.php");
?>
  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>

</center>
</body>
</html>

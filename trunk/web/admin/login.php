<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Noah Admin Login</title>
  <link href="../style/noah.css" rel="stylesheet" type="text/css" />
</head>

<body >
<center>
  <div id="bar">Login</div>
  <div id="tt">Login</div>

  <div id="main">

<?php
  if(isset($_GET['errorMsg']))
  echo "<br /><span class=cl>$_GET[errorMsg]</span><br /><br />";
?>
  <form action="login.do.php" method="post" onsubmit="encode(this.password.value)">
    <input type="hidden" name="origURL" value="<?php echo $_GET[origURL]; ?>" />
    <table width=950><tbody>
      <tr class=tro>
        <td width=450 align=right><strong>Username:&nbsp;&nbsp;</strong></td>
        <td align=left><input type="text" name="user_id" size="15" maxlength="50" /></td>
      </tr>
      <tr class=tre>
        <td align=right><strong>Password:&nbsp;&nbsp;</strong></td>
        <td align=left><input type="password" id="ps" name="password" size="15" maxlength="50" /></td>
      </tr>
      <tr class=tro>
        <td colspan=2 align=center>
        <input type="submit" value="Login" />
        </td>
	 </tr>
    </tbody></table>
  </form>
  </div>

  <div id="ft">
    <hr width="900" size=0 />
	Online Judge System of Wuhan Univ. Version 1.0<br />
    Copyright &copy; 2006 ACM/ICPC Team of Wuhan University. All rights reserved.<br />
    Please <a href="mailto:acm@whu.edu.cn?Subject=Suggestion of the OnlineJudge" >contact us</a> if you have any suggestion or problem.<br /><br />
  </div>
<script src="../common/md5.js"></script>
 <script language="javascript">
 function encode(ps)
 {
//	 document.getElementById('ps').value = MD5(ps);
	 return true;
 }
 </script>
</center>
</body>
</html>

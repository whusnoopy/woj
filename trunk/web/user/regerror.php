<?php
	include('../include/header.php');
?>

  <div id="tt">New Account Register</div>

  <div id="main">
   <div>
   <span class="cl">There is some errors occured during the Register</span>
   </div>
   <br />
   <div class="ptt">Information</div>
   <div class="code">
    <?php echo $_GET['errorMsg']; ?>
   </div>
  </div>

<?php
	include('../include/tailer.php');
?>

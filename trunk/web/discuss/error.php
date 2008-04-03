<?php
   include('../include/header.php');
   if (isset($_GET['errorMsg'])){
	   $errorMsg = $_GET['errorMsg'];
   }
   else $errorMsg = 'unknow reason!'
?>

  <div id="tt">Post Message</div>

<?php
	include('../include/notice.php');
?>

  <div id="main">
   <div>
   <span class="cl">There is some errors occured</span>
   </div>
   <br />
   <div class="ptt">Information</div>
   <div class="code">
    <?php echo $errorMsg; ?>
   </div>
  </div>

<?php
include('../include/tailer.php');
?>

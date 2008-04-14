<?php
	include('../include/header.php');
	echo '<div id=tt>Error</div>';
	include('../include/notice.php');
?>


  <div id="main">
   <br />
   <div>
	 <span class="cl">Error Occored</span>
   </div>
   <br />
   <div class="ptt">Information</div>
   <div class="code"><pre><?php echo $_GET['errorMsg']; ?></pre></div>
  </div>

<?php
	include('../include/tailer.php');
?>
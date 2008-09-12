<?php
  session_start();
  if (isset($_SESSION['user_id'])) {
    // goto page logout/modify
    header("location:modify.php");
  } else {
    // goto page login/register
    header("location:login.php");
  }
?>

<?php
	session_start();
	if (empty($_SESSION['user_id']) || $_SESSION['access'] != 'root'){
		header('Location: ../login.php?errorMsg=please login first!');
		exit;
	}

?>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Flood Admin</title>
  <link href="../../style/noah.css" rel="stylesheet" type="text/css" />
</head>
<body>
<?php
    if(get_magic_quotes_gpc()){
        foreach($_POST as &$value)
            $value = stripslashes($value);
    }

    extract($_POST);
    $contest_id = trim($contest_id);
    if(!is_numeric($contest_id)) die("Contest_id must be numeric!");
    $user_ids = trim($user_ids);
    $user_ids = explode("\n", $user_ids);
    $conn = new mysqli("localhost", "root", "123456", "flood");
    foreach($user_ids as $user_id){
        $user_id = trim($user_id);
        if (empty($user_id)) continue;
        echo "Processing $user_id<br/>\n";
        $user_id = $conn->real_escape_string($user_id);
        $query = "SELECT * FROM `contestpermission` "
                ."  WHERE `user_id`='$user_id' AND"
                ."    `contest_id`=$contest_id";
        $conn->query($query);
        if($conn->affected_rows >= 1) continue;
        $query = "INSERT INTO `contestpermission` VALUES"
                ."('$user_id', $contest_id)";
        $conn->query($query);
    }
    echo "All done. back to <a href=\"../index.php\">admin</a>";
?>
</body>
</html>

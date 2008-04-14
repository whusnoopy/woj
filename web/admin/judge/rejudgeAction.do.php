<?php

	for ($i=0; $i<20; $i++){
		$sid = 's'.$i;
		if (isset($_POST[$sid])){
			echo $_POST[$sid];
		}
	}

?>
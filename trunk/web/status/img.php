<?php
if (!isset($_GET['parm'])){
	exit;
}

$data = explode(',', base64_decode($_GET['parm']));

header("Content-type: image/gif");

$im = create_img($data);
imagegif($im);
imagedestroy($im);

?>

<?php
	function create_img($data)
	{
		$w = 210;
		$h = 320;
		$sy = 25;
		$max_value = max($data);
		if ($max_value < 2) $max_value = 2;
		if ($max_value % 2)	$max_value ++;

		$img = imagecreate($w, $h);

		$black = imagecolorallocate($img, 0, 0, 0);
		$white = imagecolorallocate($img, 0xff, 0xff, 0xff);
		$clr = array();
		$clr[0] = imagecolorallocate($img, 0xff, 0, 0);
		$clr[1] = imagecolorallocate($img, 0, 0xff, 0);
		$clr[2] = imagecolorallocate($img, 0, 0, 0xff);
		$clr[3] = imagecolorallocate($img, 0xff, 0, 0xff);
		$clr[4] = imagecolorallocate($img, 0x64, 0x64, 0x64);
		$clr[5] = imagecolorallocate($img, 0xff, 0xff, 0);
		$clr[6] = imagecolorallocate($img, 0, 0xff, 0xff);
		$clr[7] = imagecolorallocate($img, 0, 0x64, 0);
		$text = array('AC', 'PE', 'TLE', 'MLE', 'WA', 'RE', 'OLE', 'CE');

		imagefill($img, $w-1, $h-1, $white);
		imageline($img, 20, 20, $w - 10, 20, $black);
		for ($i = 60; $i < 185; $i +=40 )
			imageline($img, $i, 18, $i, 20, $black);
		imagestring($img, 2, 175, 6, $max_value, $black);
		imagestring($img, 2, 95, 6, $max_value / 2, $black);
		imageline($img, 20, 20, 20, $h - 5, $black);


		for ($i=0; $i<8; $i++){
			imagestring($img, 2, 1, 20 + ($i + 1) * 10 + $i * $sy + 7, $text[$i], $black);
			if ($data[$i] > 0)
				imagefilledrectangle($img, 21, 20 + ($i + 1) * 10 + $i * $sy, 21 + $data[$i] * 160 / $max_value, 20 + ($sy + 10) * ($i + 1), $clr[$i]);
			imagestring($img, 2, $data[$i] * 160 / $max_value + 23, 20 + ($i + 1) * 10 + $i * $sy + 7, $data[$i], $black);
		}

		return $img;
	}
?>
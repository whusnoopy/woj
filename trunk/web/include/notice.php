<?php
	if (file_exists('../common/config.php'))
		include('../common/config.php');
	else
	  include('./common/config.php');
	$filename = $DOCUMENT_ROOT.'include/note.html';
	if(!note_exist($filename))
		gen_note($filename);

	include($filename);
?>

<?php
function note_exist($filename)
{
	if (!file_exists($filename))
		return false;
	if (time() - filemtime($filename) > 60 * 1)
		return false;
	return true;
}

function get_notice()
{
	if (file_exists('../common/tcpclient.php'))
		include_once('../common/tcpclient.php');
	else
	  include_once('./common/tcpclient.php');
	$header = "np00000000";
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	$tc->connect() or die("unable to connect to server!");
	$tc->sendstr($header) or die("send header failed");
	$recv = $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0)
		$recv = $tc->recvstr($len);
	else
		$recv = null;
	$tc->close();
	return $recv;
}

function gen_note($filename)
{
	$fp = fopen($filename, 'w');
  
  // print current time for debug
	// $note = date('M j, Y  h:i:s A', time()+8*3600);
	$note = get_notice();
  if ($note != null) {
	  fwrite($fp, '
    <div id="move">
      <MARQUEE SCROLLAMOUNT=3 BEHAVIOR=ALTERNATE SCROLLDELAY=150><font color=red>'.$note.'</font></MARQUEE>
    </div>');
  } else {
    fwrite($fp, '');
  }
	fclose($fp);
}
?>

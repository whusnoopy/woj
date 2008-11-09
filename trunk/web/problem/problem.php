<?php
  session_start();
	include('../common/tcpclient.php');
	include('../common/format_output.php');
	if (isset($_GET['problem_id']))
		$problem_id = intval( $_GET['problem_id'] );
	else
		$problem_id = 0;
	if (isset($_GET['contest_id']))
		$contest_id = intval( $_GET['contest_id'] );
	else
		$contest_id = '0';

	$problem = array();
	get_problem_info($problem_id, $problem);
	if (count($problem) < 12){
		for($i = count($problem)+1; $i <= 12; $i++)
			$problem[] = ' ';
	}
	$fo = new format_output();

	include('../include/header.php');
	echo '<title>Problem</title>';
?>
  <div id="tt">
    <?php echo "Problem ".$problem_id.' - '.$problem[0];?>
  </div>

<?php
	include('../include/notice.php');
?>

  <div class="ifm">
    <strong>Time Limit</strong>:<?php echo $problem[8]; ?>MS&nbsp;
    <strong>Memory Limit</strong>:<?php echo $problem[9]; ?>KB<br>
    <strong>Total Submit</strong>:<span id="a_submit">loading...</span>&nbsp;
    <strong>Accepted</strong>:<span id="a_ac">loading...</span><br>
  </div>
<script language="javascript">
    function getXMLHTTP(){
        var xml = null;
        try{
            xml = new XMLHttpRequest;
        }catch(ie1){
            try{
                xml = new ActiveXObject("Msxml2.XMLHTTP");
            }catch(ie2){
                xml = new ActiveXObject("Microsoft.XMLHTTP");
            }
        }
        return xml;
    }
    function stateChange(){
        if(xml.readyState == 4){
            if(xml.status == 200){
                var res = xml.responseText.split("|");
                a_submit.innerHTML = res[0];
                a_ac.innerHTML = res[1];
            }else{
                a_submit.innerHTML = "N/A";
                a_ac.innerHTML = "N/A";
            }
        }
    }
/*
    var problem_id = window.location.search.substr(12, 4);
    if(problem_id < 1001){
        problem_id = window.location.pathname.substr(13, 4);
    }
alert(problem_id);
*/
    var problem_id = <?php echo $_GET['problem_id']; ?>;
    var url = "../problem/problemStatistics.php?problem_id=" + problem_id;
    var a_submit = document.getElementById("a_submit");
    var a_ac = document.getElementById("a_ac");
    var xml = getXMLHTTP();
    try{
        xml.onreadystatechange = stateChange;
        xml.open("GET", url, true);
        xml.send(null);
    }catch(e){
        alert(e);
    }
</script>

  <div id="main">
    <div class="ptt">Description</div>
    <div class="ptx" id="description"><?php echo $fo->formatHtml($problem[1]); ?></div>

    <div class="ptt">Input</div>
    <div class="ptx"><?php echo $fo->formatHtml($problem[2]); ?></div>

    <div class="ptt">Output</div>
    <div class="ptx"><?php echo $fo->formatHtml($problem[3]); ?></div>

    <div class="ptt">Sample Input</div>
    <div class="ptx"><?php echo $fo->formatHtml($problem[4]); ?></div>

    <div class="ptt">Sample Output</div>
    <div class="ptx"><?php echo $fo->formatHtml($problem[5]); ?></div>

    <div class="ptt">Hint</div>
    <div class="ptx"><?php echo $fo->formatHtml($problem[6]); ?></div>

    <div class="ptt">Source</div>
    <div class="ptx">
		<div><?php echo $fo->formatHtml($problem[7]); ?></div>
    </div>
    <br />
    <div>
	  <span class="bt"><a href="/flood/submit/submit.php?contest_id=<?php echo $contest_id?>&problem_id=<?php echo $problem_id;?>"> Submit</a></span>&nbsp;&nbsp;
	  <span class="bt"><a href="/flood/discuss/discussList.php?pid=<?php echo $problem_id;?>">Discuss</a></span>&nbsp;&nbsp;
      <span class="bt"><a href="/flood/status/problemstatus.php?problem_id=<?php echo $problem_id;?>">Status</a></span>
    </div>
    <br />
  </div>

<?php
	include('../include/tailer.php');

//---------

function get_problem_info($problem_id, &$problem)
{
	/////////////////////////////
/*	$d="\001";
	$recv = "a + b".$d."we begin".$d."a, b".$d."a+b".$d."1, 2".$d."3".$d."".$d."woj".$d."1000".$d."1024".$d."120".$d."100";
	$problem = explode("\001", $recv);
	return;
*/	//////////////////////////////

	if(empty($problem_id)){
		$problem = null;
		return;
	}

	$header = sprintf("%s%08d", "pb", strlen($problem_id));
	$tc = new TCPClient();
	$tc->create() or die("unable to create socket!");
	if (!$tc->connect()){// or die("unable to connect to server!");
//0		header('HTTP/1.1 404 Not Found');
		exit;
	}
	$tc->sendstr($header) or die("send header failed");
	$tc->sendstr($problem_id)or die("send message failed");
	$recv= $tc->recvstr(10);
	sscanf($recv, "%d", $len);
	if($len > 0){
		$recv = $tc->recvstr($len);
		$problem =  explode("\001", $recv);
	}
	else $problem = null;
	$tc->close();
	return;
}
?>

<?php
	session_start();
	include('../common/tcpclient.php');
	include('classes/discuss_list_t.php');

	if (isset($_GET['pid']) && !empty($_GET['pid']))
		$problem_id = $_GET['pid'];
	else
		$problem_id = '0';
	if (isset($_GET['cid']) && !empty($_GET['cid']))
		$contest_id = $_GET['cid'];
	else
		$contest_id = '0';
	if (isset($_GET['uid']) && strlen($_GET['uid']) > 0)
		$user_id = $_GET['uid'];
	else
	    $user_id = '?';
	if (isset($_GET['start']) && !empty($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (isset($_GET['title']) && strlen($_GET['title']))
		$title = $_GET['title'];
	else
		$title = '?';

	$dl = new discuss_list_t($title, $problem_id, $contest_id, $user_id, $start);
	$dl->getResult();
	$rows = $dl->getRow();
?>
<?php
	include('../include/header.php');
	echo '<title>Discuss List</title>';
	echo '<div id="tt">Discuss </div>';
	include('../include/notice.php');
?>
</center>
<br>
<script language="javascript">
    function changevisible(element){
        var children = element.parentNode.childNodes;
        for(var i= 0; i<children.length;i++){
            if(children[i].nodeName == "DIV"){
                var className = children[i].className;
                if(className == "show"){
                    element.className="fold";
                    children[i].className="hidden";
                }
                else{
                    element.className="expend";
                    children[i].className="show";
                }
            }
        }
    }
</script>
<style type="text/css">
div.show{
    display:block;
}
div.hidden{
    display:none;
}

span.fold{
    width:20px;
    height:20px;
    padding:0px 10px 0px 10px;
    background-image:url("F.gif");
    background-repeat:no-repeat;
 }
span.expend{
    width:20px;
    height:20px;
    padding:0px 10px 0px 10px;
    background-image:url("E.gif");
    background-repeat:no-repeat;
}
span.2blank{
    width:20px;
}
span.3blank{
    width:40px;
}
span.def{
    width:20px;
    height:20px;
    padding:0px 10px 0px 10px;
    background-image:url("D.gif");
    background-repeat:no-repeat;
}
</style>

<div id="mainp">

<?php
	$next_level = 1;
	for ($i=0; $i<$rows; $i++){
		$cur_level = $dl->getLevel($i);
		if($i && $cur_level==1){
			echo '<hr width=900 size=0/>';
		}
		if ($i<$rows-1)
			$next_level = $dl->getLevel($i + 1);
		else
			$next_level = 1;

		echo '<div class="show">';

		for ($j=1; $j<$cur_level; $j++)
			echo '&nbsp;&nbsp;';
		if ($cur_level < $next_level)
			echo '<span class="expend" onclick="changevisible(this)">&nbsp;</span>';
		else
			echo '<span class="def">&nbsp;</span>';

		echo '<a href="discuss.php?message_id='.($dl->getDiscuss_id($i)).'">'.($dl->getTitle($i)).'</a>&nbsp';
		echo '<strong>'.$dl->getUser_id($i).'</strong>&nbsp;';
		echo $dl->getIn_date($i).'&nbsp;';
		echo '<strong>Problem-'.$dl->getProblem_id($i).'</strong><br/>';

		if ($cur_level >= $next_level)
			echo '</div>';

		if ($cur_level > $next_level){
			for ($j=$next_level; $j<$cur_level; $j++)
				echo '</div>';
		}
	}
?>
</div>
<br>

<center>
<div id="main">
<form action="discussList.php" method="get">
   <strong>Select status by</strong>&nbsp;
   <strong>Problem ID:</strong> <input size=8 name="pid" />&nbsp;
   <strong>User ID:</strong> <input size=15 name="uid" />&nbsp;
   <strong>Title:</strong> <input size=8 name="title" />&nbsp;
   <input type=submit value="Go" width="8" />
</form>
  <br>

<?php
	if ($start>0){
		$pre=$start-1;
       echo "<span class=\"bt\"><a href=\"discussList.php?start=$pre&pid=$problem_id&uid=$user_id&title=$title\">Prev Page</a></span>";
	}
   echo "<span class=\"bt\"><a href=\"sendDiscuss.php?pid=$problem_id\">&nbsp;Send</a></span>";

     $next = $start+1;
     echo "<span class=\"bt\"><a href=\"discussList.php?start=$next&pid=$problem_id&uid=$user_id&title=$title\">&nbsp;Next Page</a></span>";

?>
  <br>
</div>
<center>
<?php
	include('../include/tailer.php');
?>

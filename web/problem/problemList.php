<?php
	include('../common/tcpclient.php');
	include('classes/problem_list_t.php');
	if (isset($_GET['start']))
		$start = $_GET['start'];
	else
		$start = '0';
	if (empty($user_id))
		$user_id = '?';

	$pl = new problem_list_t($start, $user_id, '?', '?');
	$pl->getResult();
	$pages = $pl->getPages();
	$rows = $pl->getRow();

	include('../include/header.php');
	echo "<div id=tt>Problems Volume $start</div>";
	include('../include/notice.php');

	for ($i=0; $i<$pages; $i++)
	    echo "<a href=\"problemList.php?start=$i\"><b>[$i] </b></a>";
	echo '<br>';
?>

<script language='javascript'>
function create_table(rows)
{
	for (i=0; i<rows; i++){
		if (i%2 == 0)
		    document.write('<tr class=tro>');
		else
			document.write('<tr class=tre>');
		document.write('<td id="'+i+'0"></td><td id="'+i+'1"></td><td id="'+i+'2"></td><td id="'+i+'3"></td></td>');
	}
}
function cmp1(pr1, pr2)
{
	if (pr1[1] < pr2[1])
		return -1;
	else if(pr1[1] > pr2[1])
		return 1;
	else
		return 0;
}
function cmp2(pr1, pr2)
{
	if (pr1[2] < pr2[2])
		return -1;
	else if(pr1[2] > pr2[2])
		return 1;
	else
		return 0;
}
function cmp3(pr1, pr2)
{
	if (pr1[3] * pr2[4] < pr1[4] * pr2[3])
		return -1;
	else if(pr1[3] * pr2[4] > pr1[4] * pr2[3])
		return 1;
	else
		return 0;
}
function cmp4(pr1, pr2)
{
	if (pr1[3] < pr2[3])
		return -1;
	else if(pr1[3] > pr2[3])
		return 1;
	else
		return 0;
}
function cmp5(pr1, pr2)
{
	if (pr1[4] < pr2[4])
		return -1;
	else if(pr1[4] > pr2[4])
		return 1;
	else
		return 0;
}

function output(of1)
{
	if (of1==1)
	    pl.sort(cmp1);
	else if (of1==2)
		pl.sort(cmp2);
	else if (of1==3)
		pl.sort(cmp3);
	else if (of1==4)
		pl.sort(cmp4);
	else if (of1==5)
		pl.sort(cmp5);

	for (i=0; i<rows; i++){
		if (pl[i][0] == 1)
			document.getElementById(i.toString() + '0').innerHTML = '<img src="../image/accepted.gif"/>';
		else if(pl[i][0] == 2)
			document.getElementById(i.toString() + '0').innerHTML = '<img src="../image/wrong.gif"/>';
		else
			document.getElementById(i.toString() + '0').innerHTML = '';
		for (j=1; j<3; j++)
		    document.getElementById(i.toString() + j.toString()).innerHTML = '<a href="problem.php?problem_id='+pl[i][1]+'">' +pl[i][j] + '</a>';
		document.getElementById(i.toString() + '3').innerHTML = pl[i][5]+'% (' + pl[i][3]+'/'+pl[i][4] + ')';
	}
}
</script>

<?php
	echo '<script language="javascript">'."\n";
	echo 'var rows = '.$rows.";\n";
	echo 'var pl = new Array(); ';
	for ($i=0; $i<$rows; $i++){
		$ac = $pl->getAC($i);
		$total = $pl->getTotal($i);
		echo 'var p = new Array();'."\n";
		echo 'p[0] = "'.$pl->getResult_type($i)."\";\n";
		echo 'p[1] = '.$pl->getProblem_id($i).";\n";
		echo 'p[2] = "'.$pl->getTitle($i)."\";\n";
		echo 'p[3] = '.$ac.";\n";
		echo 'p[4] = '.$total.";\n";
		echo 'p[5] = "'.($total=='0' ? '0.00' : sprintf("%.2f", $ac*100/$total) )."\";\n";
		echo "pl[{$i}] = p;\n";

	}
	echo '</script>';
?>

  <div id="main">

  <table><tbody align=center>
    <tr>
      <th width="80">&nbsp;</th>
      <th width="100"><a href="javascript:output(1)">ID</a></th>
      <th width="590"><a href="javascript:output(2)">Title</a></th>
      <th width="180"><a href="javascript:output(3)">Ratio</a>&nbsp;(<a href="javascript:output(4)">AC</a>/<a href="javascript:output(5)">Total</a>)</th>
    </tr>
<script language="javascript">
	create_table(rows);
	output(1);
</script>

</tbody></table>
  <br>
  <br />
  <div>
    <form method=post action="searchList.php?start=0" >
	<strong>Search:</strong>&nbsp;<input name=key type=text value='' size=50 maxlength="255" />&nbsp;
    <strong>By:</strong>&nbsp;<select name=field><option value="title">Title</option><option value="source">Source</option></select>&nbsp;
    <input type=submit value=GO />
    </form>
  </div>
  <br>
  </div>

<?php
	include('../include/tailer.php');
?>


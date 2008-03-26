<?php
	include('../include/header.php');
	echo '<div id="tt">Discuss </div>';
	include('../include/notice.php');

	if(isset($_GET['message_id']))
		$message_id = $GET['message_id'];
?>
<div id=main>
<table>
  <tbody>
  <tr class=tro>
    <th align=right width=100>Title:</th>
    <th align=left width=700>fhfhgfhjf</th></tr>
  <tr class=tre>
    <td align=right><strong>Problem:</strong></td>
    <td align=left>1001</td></tr>
  <tr class=tre>
    <td align=right><strong>From:</strong></td>
    <td align=left>magiii</td></tr>
  <tr class=tre>
    <td align=right><strong>Send Time:</strong></td>
    <td align=left>2007-07-20 20:41:43.0</td></tr>
  <tr class=tro valign=top>
    <td align=right><strong>Content:</strong></td>
    <td align=left>
      <div class=code>hello </div>
	</td>
  </tr>
   </tbody>
</table>

<br>
<span class=bt><a href="discussList.php">Return</a></span>
<span class=bt><a href="replyDiscuss.php?message_id=$message_id">Reply</a></span>
<br>
<br>
</div>



<?php
	include('../include/tailer.php');
?>
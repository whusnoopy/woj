<?php

$arr[0]['a']='0a';
$arr[0]['b']='0b';
$arr[1]['a']='1a';
$arr[1]['b']='1b';

foreach($arr as $a){
	echo "$a[a]"." ";
	echo "$a[b]"."\n";
}
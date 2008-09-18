<?php
/*

格式化UBB输出,要求待格式化的文本不可以有嵌套的标签

convert_ubb()函数取自bo-blog 2.10 /inc/ubb.php
由Felix021修改，可以使用
[img=URL地址]鼠标悬停提示[/img] 如 [img=http://a.com/1.jpg]点击查看大图[/img]
[img=URL地址] 如 [img=http://a.com/1.jpg]
[size=字体大小][/size] 如 [size=14px]这里的字体14px[/size];
[url=URL]显示内容[/url] 如 [url=http://a.com/]打开a.com[/url]
[color=颜色]显示内容[/color]
[font=字体]显示内容[/font]
[p align=<center|left|right>]内容[/p] 对齐
[b][/b]粗体
[i][/i]斜体
[u][/u]下划线
[pre][/pre]预设
[sup][/sup]上标
[sub][/sub]下标

*/
class format_output{

	function formatHtml($origHtml)
	{
        $result = $this->convert_ubb(nl2br(htmlspecialchars($origHtml)));
        $result = str_replace("   ", "&nbsp; &nbsp;", $result);
        $result = str_replace("  ", "&nbsp; ", $result);
        $result = preg_replace("/&amp;#(\d+?);/is", "&#\\1;", $result);
        return $result;
	}

    function convert_ubb ($str) {
	    $regubb_search = array(
				    "/\[img=([^\[]*)\](.+?)\[\/img\]/is",
				    "/\[img=([^\[]*)\]/is",
				    "/\[size=([^\[\<]+?)\](.+?)\[\/size\]/ie",
				    "/\[url=([^\[]*)\](.+?)\[\/url\]/is",
				    "/\[color=([a-zA-Z0-9#]+?)\](.+?)\[\/color\]/i",
				    "/\[font=([^\[\<:;\(\)=&#\.\+\*\/]+?)\](.+?)\[\/font\]/i",
				    "/\[p align=([^\[\<]+?)\](.+?)\[\/p\]/i",
				    "/\[b\](.+?)\[\/b\]/i",
				    "/\[i\](.+?)\[\/i\]/i",
				    "/\[u\](.+?)\[\/u\]/i",
				    "/\[pre\](.+?)\[\/pre\]/i",
				    "/\[sup\](.+?)\[\/sup\]/i",
				    "/\[sub\](.+?)\[\/sub\]/i",
	    );
	    $regubb_replace =  array(
				    "<img src=\"\\1\" border=\"0\" alt=\"\\2\"></img>",
				    "<img src=\"\\1\" border=\"0\"></img>",
				    "<span style=\"font-size:\\1;\">\\2</span>",
				    "<a href=\"\\1\" target=\"_blank\">\\2</a>",
				    "<span style=\"color: \\1;\">\\2</span>",
				    "<span style=\"font-family: \\1;\">\\2</span>",
				    "<p align=\"\\1\">\\2</p>",
				    "<strong>\\1</strong>",
				    "<em>\\1</em>",
				    "<u>\\1</u>",
				    "<pre>\\1</pre>",
				    "<sup>\\1</sup>",
				    "<sub>\\1</sub>",				
	    );
	    $str=preg_replace($regubb_search, $regubb_replace, $str);
	    return $str;
    }
}

?>

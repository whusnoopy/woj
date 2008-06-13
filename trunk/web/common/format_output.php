<?php

//格式化html输出,要求待格式化的文本不可以有嵌套的标签
//且待格式化文本中不可能有<pre>标签
class format_output
{

    //转化普通文本中的特殊字符,需要转化的特殊字符有:
	//<         &#60;
	//>         &#62;
	//&         &#38;
	//'         &#39;
	//"         &#34;
	//\n        <br>
	//' '       &nbsp;

	//可以识别的html标签有:
	//<font> <img>
	var $htmlTag = array("font","img");

	function change($ch)
	{
		if($ch != "<" && $ch != ">" && //$ch != "&" &&
			$ch != "\'" && $ch != "\"" && $ch != "\n" && $ch != " "){
			return $ch;
		}

		switch($ch){
			case "<": $str = "&#60;";  break;
			case ">": $str = "&#62;";  break;
		//	case "&": $str = "&#38;";  break;
			case "\'": $str = "&#39;"; break;
			case "\"": $str = "&#34;"; break;
			case "\n": $str = "<br>"; break;
			case " ": $str = "&nbsp;"; break;
		}
		return $str;
	}

	function isTag($str){
		for($i = 0;$i < count($this->htmlTag);$i++){
			if($this->htmlTag[$i] == $str) return true;
		}
		return false;
	}

	function formatHtml($origHtml)
	{
		$formatHtml = '';
		$status = 0;   //状态机转化,0表示原始状态
		$index = 0;
		$len = strlen($origHtml);
		$next = 0;
		$match = '';
		while($index < $len){
			$ch = $origHtml{$index};
			if($status == 0){		//原始状态(状态0)
				if($ch != '<'){
					$formatHtml.=($this->change($ch));
					$index++;
				}
				else{
					$next = $index+1;
					while($next < $len && ($ch=$origHtml{$next}) != ' '
					    && $ch != "\n" && $ch != '>') $next++;
					$sub = substr($origHtml,$index+1,$next-$index-1);
					if(!$this->isTag($sub)){
						$formatHtml.=$this->change($origHtml{$index});
						$index++;
					}
					else{
						$formatHtml.=("<".$sub);
						$match = $sub;
						$index = $next;
						$status = 1;
					}
				}
			}
			else if($status == 1){ //识别了半左标签状态(状态1)
				while($index < $len && $ch != '>' && $ch != '/'){
					$formatHtml.=$ch;
					$index++;
					$ch = $origHtml{$index};

				}
				if($ch == '>'){
					$formatHtml.=$ch;
					$index++;
					$status = 2;
				}
				else{
					if($index < $len && $origHtml{$index+1} == '>'){
						$formatHtml.=("/>");
						$index+=2;
						$status = 0;
					}
					else{
						$formatHtml.=$ch;
						$index++;
					}
				}
			}
			else{					//识别了左标签(状态2)
				while($index < $len && $ch != '<'){
					$formatHtml.=($this->change($ch));
					$index++;
					$ch = $origHtml{$index};
				}
				if($ch == '<' && $index < $len && $origHtml{$index+1} == '/'){
					$next = $index+2;
					while($next < $len && ($ch=$origHtml{$next}) != ' '
					    && $ch != "\n" && $ch != ">") $next++;
					$sub = substr($origHtml, $index+2,$next-$index-2);
					if($match == $sub && $origHtml{$next} == '>'){
						$formatHtml.=("</".$sub.">");
						$status = 0;
						$index = $next + 1;
					}
					else{
						$formatHtml.=($this->change($origHtml{$index}));
						$index++;
					}
				}
				else{
					$formatHtml.=($this->change($ch));
					$index++;
				}
			}
		}
		return $formatHtml;
	}


}
?>
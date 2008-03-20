
<?php

class format_code_t
{
	var $m_orig_code = null;
	var $m_language = null;
	var $m_keyword = null;

	var $cplusplus = array(
		"and","and_eq","asm","auto","bitand","bitor",
		"bool","break","case","catch","char","class",
		"compl","const","const_cast","continue","default","delete",
		"do","double","dynamic_cast","else","enum","explicit",
		"export","extern","false","float","for","friend",
		"goto","if","inline","int","long","mutable",
		"namespace","new","not","not_eq","operator","or",
		"or_eq","private","protected","public","register","reinterpret_cast",
		"return","short","signed","sizeof","static","static_cast",
		"struct","switch","template","this","throw","true",
		"try","typedef","typeid","typename","union","unsigned",
		"using","virtual","void","volatile","wchar_t","while",
		"xor","xor_eq");

	var $gnupascal = array(
		"absolute","abstract","and","and_then","array","asm","asmname","attribute",
		"begin","bindable","case","class","const","constructor",
		"destructor","div","do","downto","else","end","export","exports","external",
		"file","for", "forward", "function","goto","if","import","implementation","in",
		"inherited","inline","interface", "interrupt","is",
		"label","library","mod","module","nil","not","object","of","only",
		"operator","or","or_else","otherwise","packed","pow","private",
		"procedure","program","property","public","published","qualified",
		"record","repeat","resident","restricted","segment","set","shl","shr",
		"static","then","to","type","unit","until","uses","var","view","virtual",
		"volatile","while","with","xor" );

	var $freepascal = array(
		"absolute","and","array","asm","begin","break","case","const","constructor",
		"continue","destructor","dispose","div","do","downto","else","end","exit","false",
		 "file","for","function","goto","if","implementation","in","inherited","inline",
		"interface","label","mod","new","nil","not","object","of","on","operator","or","packed",
		"procedure","program","record","repeat","self","set","shl","shr","string",
		"then","to","true","type","unit","until","uses","var","while","with","xor" );

	var $javaLan = array(
		"abstract","boolean","break","byte","case","catch","char","class","continue",
		"default","do","double","else","extends","false","final","finally","float","for",
		"if","implements","import","instanceof","int","interface","length","long",
		"native","new","null","package","private","protected","public","return",
		"short","static","super","switch","synchronized","this","threadsafe","throw","throws",
		"transient","true","try","void","while");


	function format_code($code, $language)
	{
		$this->m_orig_code = $code;
		$this->m_language = $language;
		$this->set_language();
	}

	function set_language()
	{
		if ($this->m_language == "GCC"){
			$this->m_keyword = $this->cplusplus;
		}
		else if ($this->m_language == "G++"){
			$this->m_keyword = $this->cplusplus;
		}
		else if ($this->m_language == "Java"){
			$this->m_keyword = $this->javaLan;
		}
		else if ($this->m_language == "Free Pascal"){
			$this->m_keyword = $this->freepascal;
		}
		else $this->m_keyword = $this->gnupascal;

	}

	//test if the parameter str is in the keyword table
	function isKeyword($str)
	{
		//if(str == null) return false;
		//if(keyword == null) return false;
		$low = 0;
		$high = count($this->m_keyword) - 1;
		while($low <= $high){
			$mid = (int)(($low + $high)/2);
			if (($comp = strcmp($this->m_keyword[$mid], $str)) == 0) {return true;}
			else if($comp < 0) $low = $mid + 1;
			else $high = $mid - 1;
		}
		return false;
	}

	function isDelimit($ch)
	{
		return (!(($ch >= "0" && $ch <= "9") ||
					($ch >= "A" && $ch <= "Z") ||
					($ch >= "a" && $ch <= "z") ||
					($ch == "_")));
	}

	//change special character to html format
	function change($ch)
	{
		if($ch != "<" && $ch != ">" &&
			$ch != "\'" && $ch != "\"" && $ch != "\n" && $ch != " " && $ch != "\t"){
			return $ch;
		}
		//System.out.println("hehe:" + ch);
		switch($ch){
			case "<": $str = "&#60;";  break;
			case ">": $str = "&#62;";  break;
			//case '&': str = "&#38;";  break;
			case "\'": $str = "&#39;"; break;
			case "\"": $str = "&#34;"; break;
			case "\n": $str = "<br>";  break;
			case " ":  $str = "&nbsp;"; break;
			case "\t": $str="&nbsp;&nbsp;&nbsp;&nbsp;"; break;
		}
//		echo $str;
		return $str;
	}


	function set_orig_code($code)
	{
		$this->m_orig_code = $code;
	}

    //C和C++语言程序的格式化函数,完成两种格式化功能
	//1.注释 <i> <font color="#008000">,支持两种注释
	//2.关键字:加粗 <b>
	function getResultSource(){

		$index = 0;
		$len = strlen($this->m_orig_code);
		$format_code = "";
		//System.out.println(len);
		while($index < $len){
			$ch = $this->m_orig_code{$index};
			//System.out.println(ch);
			if ($this->isDelimit($ch)){
				if($ch != "/"){
					$format_code .= $this->change($ch);
					$index++;
				}
				else{
					if($index < $len && (($ch = $this->m_orig_code{$index+1}) == "/" || $ch == "*")){
						$format_code .= ("<i> <font color=\"#008000\">");
						$format_code .= ("/");
						$index++;
						if($ch == "/"){

							while($index < $len && $ch != "\n"){
								$ch = $this->m_orig_code{$index};
								$format_code .= ($this->change($ch));
								$index++;
							}

						}
						else{
							$format_code .= ("*");
							$index++;
							if($index < $len){
								$ch = $this->m_orig_code{$index};
						 	        while($index < $len){
									if($this->m_orig_code{$index} == '*' &&
										$index < $len && $this->m_orig_code{$index+1} == "/"){
										$format_code .= ("*/");
										$index += 2;
										break;
									}
									else{
										$format_code .= ($this->change($ch));
										$index++;
										if($index < $len) $ch = $this->m_orig_code{$index};
									}
								}
							}
						}
						$format_code .= ("</font></i>");
					}//if
					else{
						$format_code .= ("/");
						$index++;
					}
				}//else
			}//if
			else{
				$next = $index;
				while($next < $len && !($this->isDelimit($this->m_orig_code{$next}))){
					$next++;
				}
				$str = substr($this->m_orig_code, $index, $next-$index);
				if($this->isKeyword($str)){
					$format_code .= ("<b>");
					$format_code .= ($str);
					$format_code .= ("</b>");
				}
				else{
					$format_code .= ($str);
				}
				$index = $next;
			}

		}
		return $format_code;
	}
}

/*
$origcode = "#include <iostream>
using namespace std;
int main()
{
 long a,b;
 cin>>a>>b;
 cout<<a+b<<endl;
 return 0;
}";


$fc = new format_code($origcode, "G++");

echo  $fc->getResultSource();
*/
?>
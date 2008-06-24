1，php.ini的配置

For windows: (文件路径：安装目录/php.ini)
{
  error_reporting  =  E_ALL
  display_errors = Off
  magic_quotes_gpc = Off
  upload_max_filesize = 8M
  output_buffering = 4096

  extension=php_sockets.dll
  extension=php_gd2.dll
}

For ubuntu: （文件路径：/etc/php5/apache2/php.ini）
{
  error_reporting  =  E_ALL
  display_errors = Off
  magic_quotes_gpc = Off
  upload_max_filesize = 8M
  output_buffering = 4096

  安装gd扩展：
      sudo apt-get install php5-gd
  extension=gd.so

}

2, Ubuntu下设置下列文件为可写：flood/files, flood/contest/problems, flood/include/note.html 
   chmod o+w flood/files
   chmod o+w flood/contest/problems
   chmod o+w flood/include/note.html

3, 配置文件:
flood/common/config.php
flood/admin/common/config.php

4，admin,files布置在一个服务器上，其他可布置在多个服务器上

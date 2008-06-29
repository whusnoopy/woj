databasetranslator
数据库转换器
[注意]：请安装好Server后再编译databasetranslator
执行步骤如下：
修改源文件中的
s_connection = createConnection("localhost", "用户名", "密码", "源数据库名");
d_connection = createConnection("localhost", "用户名", "密码", "目的数据库名");


s_connection为源数据库
d_connection为目的数据库

2.编译databasetranslator
  执行make main命令
产生main可执行文件

执行即可

rejudge
rejudge工具
[注意]：请安装好Server后再编译databasetranslator
执行make rejudge命令
修改tools目录下的rejudgePart.sh脚本
执行./rejudgePart.sh start end
start为rejudge开始的statuses号（包括）
end为rejudge结束的statuses号（不过包括）

mostdiligenplayer
最努力的用户
执行make mdp命令
修改dayplayer.sh,weekplayer.sh,monthplayer.sh三个脚本中mdp的路径
修改cronFile文件中的路径
执行crontab cronFile命令


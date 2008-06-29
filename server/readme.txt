安装前的准备：
安装 libxml2-dev
sudo apt-get install libxm2-dev
安装 mysql-client
sudo apt-get install mysql-client
安装 libmysqlclient15-dev
sudo apt-get install libmysqlclient15-dev
安装 screen
sudo apt-get install screen
配置数据库
在mysql数据库中建个数据库，并转入改数据库
执行source [Server源文件目录绝对路径]/database.sql建立数据库结构

编译安装
进入Server源文件文件夹，make 编译
编译完成后，执行文件在源文件目录的bin文件夹下
修改bin目录下的configure.xml文件
修改源文件目录下的startOnlineJudgeServer.sh
修改其中可执行文件和配置文件(--configure_path)的绝对路径和其他参数。
使用screen ./startOnlineJudgeServer.sh执行Server, Server即启动完成。
按下Ctrl+a+d可以跳出screen界面，使得程序执行在后台，需要弄回前台时，
输入screen -r即可。


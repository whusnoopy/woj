configure_path="/home/flood/worktemp/bin/configure.xml"
root_dir="/home/flood/worktemp"

cmd="screen /home/flood/worktemp/bin/oj_server --root_dir=\"$root_dir\" --logtostderr=true --log_level=5 --configure_path=\"$configure_path\""
echo $cmd
echo `After the server start up, press Ctrl_a then press d to quit screen monitor`
eval $cmd

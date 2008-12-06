configure_path="/home/flood/worktemp/bin/configure.xml"
root_dir="/home/flood/worktemp"

cmd="screen /home/flood/floodprj/server/bin/oj_server --root_dir=\"$root_dir\" --logtostderr=false --log_level=4 --configure_path=\"$configure_path\"
echo $cmd
echo "After the server start up, press Ctrl_a then press d to exit screen monitor"
eval $cmd

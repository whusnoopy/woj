ids=`cat /etc/passwd | grep flood | awk -F ':' '{print "--uid=" $3, "--gid=" $4}'`

cmd="./$1 --root_dir=\"/tmp\" --logtostderr=true --log_level=5 --server_address=\"127.0.0.1\" --server_port=3241 $ids"
echo $cmd
eval $cmd


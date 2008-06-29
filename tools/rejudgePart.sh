#!/bin/bash

/home/flood/flood/tools/rejudge --server_ip="127.0.0.1" --server_port=3030 --start=$1 --end=$2 --root_dir="/home/flood/worktemp" --logtostderr=false --log_level=4

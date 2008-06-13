#!/bin/bash
if [[ $1 = */* ]] ; then
    cd ${1%/*}
fi
filename=${1##*/}

sed -i 's/\r//g' $filename

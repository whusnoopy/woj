#!/bin/bash
if [ $1 = */* ]; then
    cd ${1%/*}
fi
src=${1##*/}
bin=${src%.*}
case ${src##*.} in
    c)
        gcc -o $bin -ansi -fno-asm -O2 -Wall -lm -static -s -DONLINE_JUDGE $src >/dev/null
        ;;
    cc)
        g++ -o $bin -ansi -fno-asm -O2 -Wall -lm -static -s -DONLINE_JUDGE $src >/dev/null
        ;;
    pas)
        fpc -o$bin -Fe"/proc/self/fd/2" -Sd -dONLINE_JUDGE -O2 $src >/dev/null
        ;;
    *) exit -1;;
esac

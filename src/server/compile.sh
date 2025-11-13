#!/bin/env bash

#
# -> make whit not AI

# 
vertion=0

files='code2/*.c'
baseFiles='../dup/flags.c ../dup/main.c ../dup/utilse.c'

flags='-g -Wall -Werror -Wextra'

name='HCS.out'



line="cc $flags $baseFiles $files -o $name"

#
echo $line
$line
echo done
#
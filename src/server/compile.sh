#!/bin/env bash

#
# -> make whit not AI

# 
vertion=0

files='code/*.cpp'

flags='-g -Wall -Werror -Wextra'

name='HCS.out'



line="c++ $flags $files -o $name"

#
echo $line
$line
echo done
#
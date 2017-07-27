#!/bin/bash

unameOut="$(uname -s)"

case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    CYGWIN*)    machine=Cygwin;;
    MINGW*)     machine=MinGw;;
    *)          machine="UNKNOWN:${unameOut}"
esac

#if [$machine = "Mac"]
#	export CC=which gcc-5
#	export CXX=which g++-5
#then 
#fi

mkdir -p TMPcompileHiCapTools

cd TMPcompileHiCapTools

cmake .. #> cmake.out

wait

make #> make.out

wait

cd ../

#rm -r TMPcompileHiCapTools

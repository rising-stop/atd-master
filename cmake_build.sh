#!/bin/bash

Configure(){
  echo "current path dir: "
  pwd
  read -p "all files would be deleted, continue(y/n)?" choice
  case "$choice" in 
    y|Y ) rm -r *;;
    n|N ) echo "shell abort"
          exit;;
    * ) echo "shell abort" 
        exit;;
  esac

  if [ "$1" == "p" ]; then
    cmake -DUSE_CC_PEGASUS=ON -DCMAKE_TOOLCHAIN_FILE=/home/igs/Documents/UrbanArea/pegasus-sdk/nvidia/cmake/Toolchain-V5L.cmake -DVIBRANTE_PDK:STRING=/home/igs/Documents/UrbanArea/pegasus-sdk/nvidia/nvidia_sdk/DRIVE/Linux/5.0.13.2/SW/DriveSDK/drive-t186ref-linux ..
    exit
  fi

  if [ "$1" == "x" ]; then
    cmake ..
  fi
}

Build(){
  make -j8 -l8
}

if [ ! -d "build" ]; then
  mkdir build
fi
cd build
if [ "$?" != "0" ]; then
  echo "working path error"
  exit
fi

while getopts ":bc:" opt
do
    case $opt in
        c)
	  case $OPTARG in
	    pegasus)
	      Configure p
	      exit;;
	    x86)
	      Configure x
	      exit;;
	    ?)
	      echo "arg wrong"
	      exit;;
	  esac
	  ;;
        b)
	  Build
	  exit;;
        ?)
        echo "unknow param"
        exit 1;;
    esac
done

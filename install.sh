#!/bin/sh

if ! [ $(id -u) = 0 ]; then
   echo "The script need to be run as root." >&2
   exit 1
fi

if [ $SUDO_USER ]; then
    real_user=$SUDO_USER
else
    real_user=$(whoami)
fi

apt-get install curl

sudo -u $real_user curl -O https://golang.org/dl/go1.17.3.linux-amd64.tar.gz

sudo -u $real_user go install github.com/bazelbuild/bazelisk@latest

apt-get install libsdl2-2.0-0
apt-get install libsdl2-dev

sudo -u $real_user ./bazelisk build //main:main

sudo -u $real_user echo "Done"

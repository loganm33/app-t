#!/bin/bash
if [[ $UID != 0 ]]; then
    echo "Please run this script with sudo:"
    echo "sudo $0 $*"
    exit 1
fi

yum -y update
yum -y upgrade
yum -y install qt5-qtbase-devel.x86_64

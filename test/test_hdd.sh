#!/bin/bash

create_cmd=""
target=$1
test_img="test/img/hda.img"

function remove_old_file() 
{
    if [ -f $test_img ]; then
	echo "Remove $test_img"
	rm $test_img
    fi
}


if [ "$target" = "" ]; then
    target="bochs"
fi

if [ "$target" = "kvm" ]; then
    create_cmd="/usr/bin/kvm-img"
    cmd="$create_cmd create -f qcow2 $test_img 10M"
elif [ "$target" = "qemu" ]; then
    create_cmd="/usr/bin/qemu-img"
    cmd="$create_cmd create -f qcow2 $test_img 10M"
elif [ "$target" = "bochs" ]; then
    create_cmd="/usr/bin/bximage"
    cmd="$create_cmd -q -hd -size=10 -mode=flat $test_img"
else
    create_cmd="/bin/dd"
    cmd="$create_cmd if=/dev/zero of=$test_img bs=1M count=10"    
fi

remove_old_file

echo $cmd
$cmd

/sbin/mke2fs -q $test_img

echo "Done."

exit 0

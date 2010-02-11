#!/bin/bash

create_cmd=""

have_qemu=0

test_img="test/img/hda.img"

function do_create_by_qemu()
{
    cmd="$create_cmd create -f qcow2 $test_img 10M"
    echo $cmd
    $cmd

}

function do_create_by_dd()
{
    cmd="dd if=/dev/zero of=$test_img bs=1M count=10"
    echo $cmd
    $cmd
}

function create_hdd_image()
{

    if [ $have_qemu = 1 ]; then
	do_create_by_qemu
    else
	do_create_by_dd
    fi
}

function remove_old_file() 
{
    if [ -f $test_img ]; then
	echo "Remove $test_img"
	rm $test_img
    fi
}

if [ -f "/usr/bin/kvm-img" ]; then
    create_cmd="/usr/bin/kvm-img"
    have_qemu=1
elif [ -f "/usr/bin/qemu-img" ]; then
    create_cmd="/usr/bin/qemu-img"
    have_qemu=1
else
    create_cmd="/bin/dd"
fi

remove_old_file
create_hdd_image

echo "Done."

exit 0

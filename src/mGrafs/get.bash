#!/bin/bash

mgrafs_dir=~/progs/iPhone/mGrafs

script_dir=${0%/*}

echo "importando mGrafs"

mg_classes=( MG_Clock MG_Control MG_GrafTexture MG_Screen )
for c in ${mg_classes[*]}
do
    cp $mgrafs_dir/src/mGrafs/$c.h $script_dir/
    cp $mgrafs_dir/src/mGrafs/$c.m $script_dir/
done



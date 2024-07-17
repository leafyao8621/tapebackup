#!/bin/bash

for i in {1..10}; do 
    dd if=/dev/random of=`printf "file_%02d" $i` bs=64K count=15000 status=progress
done

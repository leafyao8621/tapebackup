#!/bin/bash

../tbcli -p ~/test/ -v write ~/dummy.img
../tbcli -p . -v read ~/dummy.img
../tbcli read -v ~/dummy.img
../tbcli -b "2024-07-01" -e "2024-08-01" report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -x daily.txt report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV -x daily.csv report daily
../tbcli -b "2024-07-01" -e "2024-08-01" report list
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV report list
../tbcli -b "2024-07-01" -e "2024-08-01" -x list.txt report list
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV -x list.csv report list
../tbcli -p tes report lookup
../tbcli -p tes -f CSV report lookup
../tbcli -p tes -x lookup.txt report lookup
../tbcli -p tes -f CSV -x lookup.csv report lookup

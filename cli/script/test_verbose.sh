#!/bin/bash

../tbcli -p ~/test/ -v write ~/dummy.img
../tbcli -p . -v read ~/dummy.img
../tbcli read -v ~/dummy.img
../tbcli -b "2024-07-01" -e "2024-08-01" report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -x daily.txt report daily
../tbcli -b "2024-07-01" -e "2024-08-01" -f CSV -x daily.csv report daily

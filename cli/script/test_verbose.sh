#!/bin/bash

../tbcli -p ~/test/ -v write ~/dummy.img
../tbcli -p . -v read ~/dummy.img
../tbcli read -v ~/dummy.img
sqlite3 ~/.tb.db < query.sql

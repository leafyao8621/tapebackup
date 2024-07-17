#!/bin/bash

../tbcli -p ~/test/ write ~/dummy.img
../tbcli -p . read ~/dummy.img
../tbcli read ~/dummy.img

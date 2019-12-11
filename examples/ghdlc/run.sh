#!/usr/bin/env sh

set -e

ghdl -a -O0 -g tb_Cinterface.vhd
ghdl -e -O0 -g -Wl,-I./ -Wl,main.c tb_Cinterface
./tb_Cinterface

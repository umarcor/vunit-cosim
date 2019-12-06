#!/usr/bin/env sh

set -e

cd $(dirname $0)

ghdl -a -O0 -g tb_ghdlc.vhd
ghdl -e -O0 -g -Wl,-I../../cosim/ghdlc/c -Wl,src/main.c tb_ghdlc
./tb_ghdlc

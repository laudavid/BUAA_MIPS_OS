#!/bin/bash
make clean
make
gxemul -E oldtestmips -C R3000 -M 64 -d gxemul/fs.img gxemul/vmlinux

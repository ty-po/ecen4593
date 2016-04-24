#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  cat ../test/traces/$1 | ../bin/ecen4593final $2 $1 > .output.tmp
  colordiff .output.tmp ../test/output/$1/$1.$2 -w
fi

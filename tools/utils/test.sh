#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  cat ../tools/traces/$1 | ../bin/ecen4593final $2 $1 > .output.tmp
  colordiff .output.tmp ../tools/sample_output/$1/$1.$2 -w
fi

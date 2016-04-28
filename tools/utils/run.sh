#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  mkdir ../tools/output/$1
  for config in default L1-2way All-2way All-4way L1-8way L1-small L1-small-4way All-small All-FA; do
    cat ../tools/traces/$1 | ../bin/ecen4593final $config $1 > ../tools/output/$1/$1.$config
  done 
fi

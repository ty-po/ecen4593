#!/bin/bash
cd ../../src
make
echo $1 Diff Report > ../tools/utils/out.txt 

if [ $? -eq 0 ]; then
  for config in default L1-2way All-2way All-4way L1-8way L1-small L1-small-4way All-small All-FA; do
    echo -----------------------------------------  >> ../tools/utils/out.txt
    echo $config $1                                 >> ../tools/utils/out.txt
    echo -----------------------------------------  >> ../tools/utils/out.txt

    cat ../tools/traces/$1 | ../bin/ecen4593final $config $1 > ../tools/output/$1/$1.$config
    colordiff ../tools/output/$1/$1.$config ../tools/sample_output/$1/$1.$config -w >> ../tools/utils/out.txt
  done 
fi

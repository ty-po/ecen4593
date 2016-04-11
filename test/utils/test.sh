#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  cd ../bin
  cat ../test/traces/tr1 | ./ecen4593final >> output
  #colordiff ../test/output/tr1.All-FA.txt output
  colordiff ../test/output/tr1.default.txt output -w
  rm output
fi

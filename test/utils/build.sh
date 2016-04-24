#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  cat ../test/traces/$1 | ../bin/ecen4593final $2
fi

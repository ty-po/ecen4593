#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  cat ../test/traces/tr1 | ../bin/ecen4593final
fi

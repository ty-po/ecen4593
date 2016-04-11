#!/bin/bash
cd ../../src
make
if [ $? -eq 0 ]; then
  ../bin/ecen4593final
fi

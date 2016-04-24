#!/bin/bash

chsum1=""

while [[ true ]]; do
  chsum2=`sha1sum ../../src/*.cc ../../src/*.h`
  if [[ $chsum1 != $chsum2 ]]; then
    if [[ $1 == test ]]; then
      ./test.sh $2 $3
    else
      ./build.sh $1 $2
    fi
    chsum1=$chsum2
  fi
  sleep 5
done
  

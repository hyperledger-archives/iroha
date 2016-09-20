#!/bin/sh

total=0
for file in  build/test_bin/*; do
  ./${file}
  total=$((total + $?)) 
done
exit $total

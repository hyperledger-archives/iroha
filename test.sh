#!/bin/sh

total=0
for file in  build/test_bin/*_test; do
  ./${file}
  total=$((total + $?)) 
done
exit $total

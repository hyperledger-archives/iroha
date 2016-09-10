#!/bin/sh

for file in  build/test_bin/*; do
    ./${file}
done

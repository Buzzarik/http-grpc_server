#!/bin/bash

bash ./lib/app/src/generate.sh && echo 'erty'

if [ ! -d build ]; then
  mkdir build
fi

cd build && cmake ../ && make

./app/app

exit 0

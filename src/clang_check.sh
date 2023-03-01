#!/bin/bash
path="$(pwd)/../materials/linters/.clang-format"

function traverse_check {
  for dir in */; do
    if [ -d "$dir" ] && [ "$dir" != "cmake-build-debug/" ] && [ "$dir" != "qcustomplot/" ]; then
        echo "$dir"
        cd "$dir"
        cp $path .
#        if [ "$(ls *.cc)" ]; then
          clang-format -n --verbose *.cc *.h
#        fi
#        if [ "$(ls *.h)" ]; then
#          clang-format -n --verbose *.h
#        fi
        if [ "$(ls -A)" ]; then
          traverse_check
        fi
        rm .clang-format
        cd ..
    fi
  done;
}

traverse_check
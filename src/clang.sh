#!/bin/bash

path=$(pwd)/../materials/linters/.clang-format

function traverse {
  for dir in */ ; do
    if [ -d "$dir" ] && [ $dir != "cmake-build-debug/" ]; then
      echo "Running script in directory: $dir"
      cd "$dir"
      cp $path .
      clang-format -i --sort-includes --verbose --style="Google" *.cc
      clang-format -i --sort-includes --verbose --style="Google" *.h
      clang-format -i --sort-includes --verbose --style="Google" *.cpp
      if [ "$(ls -A)" ]; then
        traverse
      fi
      rm .clang-format
      cd ..
    fi
  done
}

traverse

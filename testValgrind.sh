#!/bin/bash
source build.sh
valgrind --tool=exp-sgcheck -v ./block_rb 
valgrind --tool=memcheck --leak-check=full --leak-resolution=high -v ./block_rb

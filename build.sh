#!/bin/bash

set -xe

mkdir -p bin
gcc -Wall -Wextra -ggdb -o bin/test src/test.c

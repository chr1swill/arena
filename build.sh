#!/bin/bash

set -xe

cc=tcc

mkdir -p bin
"$cc" -Wall -Wextra -ggdb -o bin/test src/test.c

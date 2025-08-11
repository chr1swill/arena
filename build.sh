#!/bin/bash

set -xe

mkdir -p bin
gcc -Wall -Wextra -ggdb -o bin/main src/main.c

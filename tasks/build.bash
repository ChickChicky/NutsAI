#!/usr/bin/env bash

g++ -o out -I ./ -L ./ *.cpp  -Wno-multichar -fpermissive "$@"
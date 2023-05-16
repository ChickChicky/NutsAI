#!/usr/bin/env bash

g++ -o out -I . *.cpp -lncurses -pthread -fpermissive -g && ./out
#!/bin/bash
PYTHONMALLOC=malloc valgrind -s --leak-check=full --show-leak-kinds=all python3 test.py

#!/usr/bin/env bash
set -e

echo -n "Compiling..."
if g++ -std=c++23 -o test.out testsuite.cpp ../bigInteger.cpp; then
	echo "OK"
else
	echo "FAILED"
	exit 1
fi

echo "Running tests..."
./test.out

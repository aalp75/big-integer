#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TEST_DIR="$ROOT_DIR/tests"

echo -n "Compiling..."
g++ -std=c++23 \
	-O2 -g \
	-pipe \
	-Wall -Wextra -Wpedantic \
	-Wshadow -Wconversion -Wsign-conversion \
	-Wformat=2 -Wundef -Wcast-qual -Wcast-align \
	-Wduplicated-cond -Wduplicated-branches -Wlogical-op \
	-Wnull-dereference \
	-Wnon-virtual-dtor -Woverloaded-virtual \
	-Wno-unused-parameter \
	-fno-omit-frame-pointer \
	-o "$TEST_DIR/test.out" \
  	"$TEST_DIR/testsuite.cpp" \
  	"$ROOT_DIR/bigInteger.cpp"
echo "OK"

echo "Running tests..."
"$TEST_DIR/test.out"

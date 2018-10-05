#!/bin/sh

echo "Building..."
make
echo "\n\n"

echo "Running main...\n\n"
./bin/main
echo "\n\nFinished running main..."

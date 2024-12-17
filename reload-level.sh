#!/bin/bash

# Recompile Level.cpp and run the game
echo "Rebuilding Level.cpp..."
if make rebuild-level; then
    echo "Rebuild successful! Running the game..."
    make run
else
    echo "Rebuild failed! Fix errors before running."
    exit 1
fi

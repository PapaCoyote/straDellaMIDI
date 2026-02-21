#!/bin/bash

# Clean build script for straDellaMIDI
# This script removes all build artifacts for a fresh build

echo "=================================="
echo "straDellaMIDI Clean Script"
echo "=================================="
echo ""

# Change to the build directory
cd "$(dirname "$0")/Builds/MacOSX"

echo "Removing build artifacts..."

# Remove build directory
if [ -d "build" ]; then
    rm -rf build/
    echo "✓ Removed build/ directory"
fi

# Remove derived data
if [ -d "$HOME/Library/Developer/Xcode/DerivedData" ]; then
    find "$HOME/Library/Developer/Xcode/DerivedData" -name "straDellaMIDI-*" -type d -exec rm -rf {} + 2>/dev/null || true
    echo "✓ Removed DerivedData"
fi

echo ""
echo "Clean complete!"
echo "Run ./build.sh to rebuild from scratch"

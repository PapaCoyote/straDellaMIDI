#!/bin/bash

# Build script for straDellaMIDI
# This script builds the plugin targets in the correct order to avoid
# the VST3 helper dependency issue.

set -e  # Exit on error

echo "=================================="
echo "straDellaMIDI Build Script"
echo "=================================="
echo ""

# Change to the build directory
cd "$(dirname "$0")/Builds/MacOSX"

# Configuration (Debug or Release)
CONFIG="${1:-Debug}"

echo "Building in $CONFIG configuration..."
echo ""

# Step 1: Build VST3 helper first
echo "Step 1/3: Building juce_vst3_helper..."
if xcodebuild -scheme "juce_vst3_helper" -configuration "$CONFIG" build 2>&1 | grep -q "Build succeeded"; then
    echo "✓ juce_vst3_helper built successfully"
else
    echo "⚠ Warning: juce_vst3_helper build had issues, but continuing..."
fi
echo ""

# Step 2: Build AU plugin
echo "Step 2/3: Building AU plugin..."
if xcodebuild -scheme "straDellaMIDI - AU" -configuration "$CONFIG" build; then
    echo "✓ AU plugin built successfully"
else
    echo "✗ AU plugin build failed"
    exit 1
fi
echo ""

# Step 3: Build VST3 plugin (now that helper exists)
echo "Step 3/3: Building VST3 plugin..."
if xcodebuild -scheme "straDellaMIDI - VST3" -configuration "$CONFIG" build; then
    echo "✓ VST3 plugin built successfully"
else
    echo "✗ VST3 plugin build failed"
    exit 1
fi
echo ""

echo "=================================="
echo "Build Complete!"
echo "=================================="
echo ""
echo "Built plugins:"
echo "  AU:   build/$CONFIG/straDellaMIDI.component"
echo "  VST3: build/$CONFIG/straDellaMIDI.vst3"
echo ""

# Check file sizes
if [ -f "build/$CONFIG/straDellaMIDI.component/Contents/MacOS/straDellaMIDI" ]; then
    AU_SIZE=$(du -h "build/$CONFIG/straDellaMIDI.component/Contents/MacOS/straDellaMIDI" | cut -f1)
    echo "AU plugin size: $AU_SIZE"
fi

if [ -f "build/$CONFIG/straDellaMIDI.vst3/Contents/MacOS/straDellaMIDI" ]; then
    VST3_SIZE=$(du -h "build/$CONFIG/straDellaMIDI.vst3/Contents/MacOS/straDellaMIDI" | cut -f1)
    echo "VST3 plugin size: $VST3_SIZE"
fi
echo ""

echo "To install plugins:"
echo "  AU:   cp -r build/$CONFIG/straDellaMIDI.component ~/Library/Audio/Plug-Ins/Components/"
echo "  VST3: cp -r build/$CONFIG/straDellaMIDI.vst3 ~/Library/Audio/Plug-Ins/VST3/"

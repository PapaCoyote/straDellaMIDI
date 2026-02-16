#!/bin/bash
# Build verification script for straDellaMIDI
# This script checks if the build files have been properly regenerated with Projucer

set -e

echo "🔍 Checking straDellaMIDI build configuration..."
echo ""

# Check if we're in the right directory
if [ ! -f "straDellaMIDI.jucer" ]; then
    echo "❌ Error: straDellaMIDI.jucer not found!"
    echo "   Please run this script from the repository root directory."
    exit 1
fi

echo "✅ Found straDellaMIDI.jucer"

# Check if MacOSX build directory exists
if [ ! -d "Builds/MacOSX" ]; then
    echo "❌ Error: Builds/MacOSX directory not found!"
    exit 1
fi

echo "✅ Found Builds/MacOSX directory"

# Check if Xcode project exists
if [ ! -d "Builds/MacOSX/straDellaMIDI.xcodeproj" ]; then
    echo ""
    echo "❌ XCODE PROJECT NOT FOUND!"
    echo ""
    echo "   The Xcode project has not been generated yet."
    echo "   You need to generate it using Projucer:"
    echo ""
    echo "   1. Open straDellaMIDI.jucer in Projucer"
    echo "   2. Set your JUCE modules path (File → Global Paths)"
    echo "   3. Save the project (Cmd+S)"
    echo ""
    echo "   See REGENERATE_BUILD_FILES.md for detailed instructions."
    echo ""
    exit 1
fi

echo "✅ Found Xcode project"

# Check for Info-AU.plist (indicates AU plugin target)
if [ ! -f "Builds/MacOSX/Info-AU.plist" ]; then
    echo ""
    echo "❌ WARNING: Info-AU.plist not found!"
    echo "   This indicates the project may not be configured for AU plugins."
    echo "   Please regenerate the build files with Projucer."
    exit 1
fi

echo "✅ Found Info-AU.plist (AU plugin configured)"

# Check for Info-VST3.plist (indicates VST3 plugin target)
if [ ! -f "Builds/MacOSX/Info-VST3.plist" ]; then
    echo ""
    echo "❌ WARNING: Info-VST3.plist not found!"
    echo "   This indicates the project may not be configured for VST3 plugins."
    echo "   Please regenerate the build files with Projucer."
    exit 1
fi

echo "✅ Found Info-VST3.plist (VST3 plugin configured)"

# Check for targets in the Xcode project
echo ""
echo "🔍 Checking Xcode targets..."

# List all targets
TARGETS=$(xcodebuild -project "Builds/MacOSX/straDellaMIDI.xcodeproj" -list 2>/dev/null | grep -A 100 "Targets:" | grep -B 100 "Build Configurations:" | grep -v "Targets:\|Build Configurations:" | sed 's/^[[:space:]]*//')

if [ -z "$TARGETS" ]; then
    echo "⚠️  WARNING: Could not list Xcode targets"
else
    echo ""
    echo "Available targets:"
    echo "$TARGETS" | while IFS= read -r target; do
        if [ -n "$target" ]; then
            if echo "$target" | grep -iq "AU"; then
                echo "   ✅ $target (Audio Unit plugin - BUILD THIS)"
            elif echo "$target" | grep -iq "VST3"; then
                echo "   ✅ $target (VST3 plugin - BUILD THIS)"
            elif echo "$target" | grep -iq "All"; then
                echo "   ✅ $target (builds all plugins - OR BUILD THIS)"
            elif echo "$target" | grep -iq "Shared.*Code\|Shared Code"; then
                echo "   ℹ️  $target (static library used by plugins - auto-built, don't build directly)"
            elif echo "$target" | grep -iq "Standalone\|App"; then
                echo "   ⚠️  $target (standalone app - DO NOT BUILD THIS for plugin)"
            else
                echo "   ℹ️  $target"
            fi
        fi
    done
fi

# Check if we have Main.cpp in the project (indicates app target might exist)
if grep -q "Main.cpp" "Builds/MacOSX/straDellaMIDI.xcodeproj/project.pbxproj" 2>/dev/null; then
    echo ""
    echo "⚠️  WARNING: Main.cpp found in Xcode project!"
    echo "   This file is for standalone apps, not plugins."
    echo "   If you see a 'Standalone' or 'App' target above, DO NOT build it."
    echo "   Build the 'AU', 'VST3', or 'All' target instead."
fi

# All checks passed
echo ""
echo "✅ BUILD CONFIGURATION LOOKS GOOD!"
echo ""
echo "⚠️  CRITICAL: When building in Xcode, make sure you select the correct target!"
echo ""
echo "   In Xcode:"
echo "   1. Select Product → Scheme → Choose 'straDellaMIDI - AU' or 'straDellaMIDI - VST3'"
echo "   2. Or build 'straDellaMIDI - All' to build both plugins"
echo "   3. DO NOT build 'straDellaMIDI - Standalone' or 'straDellaMIDI - App' if they exist"
echo ""
echo "   From command line:"
echo "   cd Builds/MacOSX"
echo "   xcodebuild -project straDellaMIDI.xcodeproj -scheme \"straDellaMIDI - AU\" -configuration Release"
echo "   xcodebuild -project straDellaMIDI.xcodeproj -scheme \"straDellaMIDI - VST3\" -configuration Release"
echo ""
echo "   Expected output in Builds/MacOSX/build/Release/:"
echo "   - straDellaMIDI.component (AU plugin, ~50-100MB, properly linked)"
echo "   - straDellaMIDI.vst3 (VST3 plugin, ~50-100MB, properly linked)"
echo ""
echo "   ❌ NOT an 8KB .component file!"
echo "   ❌ NOT a .app file (that's the standalone, not the plugin!)"
echo ""

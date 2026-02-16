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

# Check if we have Main.cpp (shouldn't be there for plugin-only project)
if grep -q "Main.cpp" "Builds/MacOSX/straDellaMIDI.xcodeproj/project.pbxproj" 2>/dev/null; then
    echo ""
    echo "⚠️  WARNING: Main.cpp found in project!"
    echo "   This indicates the project might still have standalone app configuration."
    echo "   Try regenerating the build files with Projucer."
fi

# All checks passed
echo ""
echo "✅ BUILD CONFIGURATION LOOKS GOOD!"
echo ""
echo "   You can now build the plugins with:"
echo "   cd Builds/MacOSX"
echo "   xcodebuild -project straDellaMIDI.xcodeproj -configuration Release"
echo ""
echo "   Expected output:"
echo "   - straDellaMIDI.component (AU plugin, ~50-100MB)"
echo "   - straDellaMIDI.vst3 (VST3 plugin)"
echo ""
echo "   NOT an 8KB .component file!"
echo ""

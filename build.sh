#!/bin/bash

# TD-NeuroMap CHOP Build Script for macOS
# This script builds the TouchDesigner plugin

echo "Building NeuroMap CHOP for TouchDesigner..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13

# Build the project
make -j$(sysctl -n hw.ncpu)

# Check if build was successful
if [ -f "NeuroMapCHOP.plugin" ]; then
    echo "✅ Build successful!"
    echo "Plugin location: $(pwd)/NeuroMapCHOP.plugin"
    
    # Create Plugins directory if it doesn't exist
    mkdir -p "../../Plugins"
    
    # Copy plugin to Plugins directory
    cp -r "NeuroMapCHOP.plugin" "../../Plugins/"
    
    echo "✅ Plugin copied to: $(realpath ../../Plugins)/NeuroMapCHOP.plugin"
    echo ""
    echo "📋 Next steps:"
    echo "1. Copy the plugin to your TouchDesigner Plugins folder"
    echo "2. Restart TouchDesigner"
    echo "3. Create a new CHOP node and set Type to 'Neuromap'"
    echo ""
else
    echo "❌ Build failed!"
    echo "Check the output above for errors"
    exit 1
fi
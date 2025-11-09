#!/bin/bash

# Build script for XGUI Complex Widget Example
# This script builds the complex-example application

set -e  # Exit on error

echo "======================================"
echo "XGUI Complex Widget Example - Build"
echo "======================================"
echo ""

# Check for required dependencies
echo "Checking dependencies..."

if ! pkg-config --exists gtk+-3.0; then
    echo "ERROR: GTK+ 3.0 development libraries not found!"
    echo ""
    echo "Please install GTK+ 3.0 development packages:"
    echo ""
    echo "On Ubuntu/Debian:"
    echo "  sudo apt-get install libgtk-3-dev"
    echo ""
    echo "On Fedora/RHEL:"
    echo "  sudo dnf install gtk3-devel"
    echo ""
    echo "On Arch Linux:"
    echo "  sudo pacman -S gtk3"
    echo ""
    exit 1
fi

echo "✓ GTK+ 3.0 found: $(pkg-config --modversion gtk+-3.0)"
echo ""

# Build the application
echo "Building complex-example..."
cd ../..  # Go to repository root

make -f Makefile.gtk complex-example

echo ""
echo "======================================"
echo "Build completed successfully!"
echo "======================================"
echo ""
echo "Run the application with:"
echo "  ./output/complex-example"
echo ""

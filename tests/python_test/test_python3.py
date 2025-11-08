#!/usr/bin/env python3
"""
Simple Python3 test for xgui bindings
Tests basic functionality without requiring a display
"""

import sys
import os

# Add output directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../output'))

def test_import():
    """Test that the module can be imported"""
    print("Testing import...")
    try:
        import xgui
        print("✓ xgui module imported successfully")
        return True
    except ImportError as e:
        print(f"✗ Failed to import xgui: {e}")
        return False

def test_basic_api():
    """Test basic API without GUI"""
    print("\nTesting basic API...")
    try:
        import xgui

        # Test Master class access
        print("  - Accessing Master class...")
        master = xgui.Master
        print("  ✓ Master class accessible")

        # Test constants
        print("  - Testing constants...")
        evt = xgui.EVT_BLOCK
        print(f"  ✓ EVT_BLOCK constant = {evt}")

        return True
    except Exception as e:
        print(f"  ✗ Basic API test failed: {e}")
        return False

def test_file_extension():
    """Test FileExtension class"""
    print("\nTesting FileExtension class...")
    try:
        import xgui

        ext = xgui.FileExtension("Text files", "txt")
        print("  ✓ FileExtension created successfully")
        return True
    except Exception as e:
        print(f"  ✗ FileExtension test failed: {e}")
        return False

def main():
    """Run all tests"""
    print("=" * 60)
    print("XGUI Python3 Bindings Test Suite")
    print("=" * 60)

    results = []

    # Test import
    results.append(("Import", test_import()))

    # Only continue if import succeeded
    if results[0][1]:
        results.append(("Basic API", test_basic_api()))
        results.append(("FileExtension", test_file_extension()))

    # Summary
    print("\n" + "=" * 60)
    print("Test Summary:")
    print("=" * 60)

    passed = sum(1 for _, result in results if result)
    total = len(results)

    for test_name, result in results:
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"{test_name:20s} {status}")

    print("-" * 60)
    print(f"Total: {passed}/{total} tests passed")
    print("=" * 60)

    return passed == total

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)

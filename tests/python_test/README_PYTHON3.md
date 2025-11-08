# Python3 Test Scripts

## Available Tests

### 1. test_python3.py
Basic API test that checks:
- Module import
- Basic API access
- FileExtension class

**Usage:**
```bash
python3 test_python3.py
```

**Note:** Currently fails due to GTK runtime symbols. This is expected until GTK3 migration is complete.

### 2. gui_example_python3.py
Full GUI example demonstrating:
- Window creation
- Widget creation (labels, buttons, entries)
- Event callbacks
- Layout management

**Usage:**
```bash
# With display
python3 gui_example_python3.py

# Headless (with Xvfb)
xvfb-run python3 gui_example_python3.py
```

**Requires:**
- DISPLAY environment variable
- GTK3 compatibility (pending C++ migration)

## Test Status

✅ Python bindings compiled successfully  
⚠️ Runtime tests pending GTK3 C++ migration

The Python3 bindings are fully modernized and compile without errors. The test failures are due to GTK API compatibility issues in the C++ implementation, not the Python bindings themselves.

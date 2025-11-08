# XGUI

A lightweight, cross-platform GUI library for C++ with Python bindings support.

## Overview

XGUI is a modern GUI toolkit that enables developers to build desktop applications with native look and feel across multiple platforms. With its event-driven architecture and comprehensive widget library, XGUI provides everything you need to create sophisticated user interfaces.

## Key Features

- **Cross-Platform** - Native support for Linux (GTK+), macOS, and GNUstep
- **Rich Widget Library** - Comprehensive set of UI components including buttons, labels, entries, trees, lists, and more
- **Event-Driven Architecture** - Flexible callback system for handling user interactions
- **Dynamic Properties** - Runtime property configuration for all widgets using a simple `set()`/`get()` API
- **Container-Based Layout** - Flexible layout management with HBox, VBox, Frame, and Window containers
- **Python Bindings** - Optional Python wrapper for rapid prototyping and scripting
- **Customizable Styling** - StyleManager for theming and appearance customization

## Quick Start

```cpp
#include <xgui/xgui.h>

int main() {
    // Initialize XGUI
    xgui::Master *master = xgui::Master::Create();

    // Create a window
    xgui::Window *window = master->CreateWindow();
    window->set("title", "Hello XGUI");
    window->set("width", "400");
    window->set("height", "300");

    // Create a button
    xgui::Button *button = master->CreateButton();
    button->set("text", "Click Me!");

    // Add button to window
    window->add(button);

    // Show window and run
    window->set("visible", "1");
    master->Run();

    return 0;
}
```

## Building from Source

### Prerequisites

#### Linux/GTK+
- `libgtk+-2.0-dev`
- `swig` (optional, for Python bindings)
- `python-dev` (optional, for Python bindings)

#### macOS
- Xcode Command Line Tools
- `swig` (optional, via Homebrew)
- `python3` (optional, via Homebrew)

### Compilation

1. Clone the repository:
```bash
git clone https://github.com/fsoft72/xgui.git
cd xgui
```

2. Choose and link the appropriate Makefile for your platform:
```bash
# For GTK+ (Linux)
ln -s Makefile.gtk Makefile

# For macOS
ln -s Makefile.osx Makefile

# For GNUstep
ln -s Makefile.gnustep Makefile
```

3. Build the library:
```bash
make
```

4. (Optional) Build Python bindings:
```bash
make python
```

For detailed compilation instructions, see:
- [COMPILING.md](COMPILING.md) - General compilation guide
- [COMPILING_MACOS.md](COMPILING_MACOS.md) - macOS-specific instructions

## Documentation

Comprehensive documentation is available, including:

- **[Getting Started Guide](docs/getting-started.md)** - Learn the basics and build your first application
- **[API Reference](docs/api/index.md)** - Complete documentation for all classes and widgets
- **Full Documentation Site** - Build with `mkdocs serve` from the project root

### Main Components

- **Core Classes**: Object, Widget, Master, StyleManager
- **Widgets**: Button, Calendar, Checkbox, Entry, Label, Progressbar, Slider, and more
- **Containers**: Window, Frame, HBox, VBox, Tab
- **Views**: List, Tree, Combobox
- **Callbacks**: Event handling system with multiple callback types

## Architecture

All UI components inherit from the `Widget` class, which provides a consistent API for property management and event handling. The `Master` object serves as the central factory and event loop manager.

```
Object
├── Widget (Button, Label, Entry, ...)
│   └── Container (Window, HBox, VBox, ...)
│       └── View (List, Tree, Combobox)
├── Master
├── Image
├── StyleManager
└── TreeNode
```

## Platform Support

| Platform | Backend | Status |
|----------|---------|--------|
| Linux | GTK+ 2.0 | Supported |
| macOS | Native (Cocoa) | Supported |
| GNUstep | GNUstep | Supported |

## Examples

### JSON Examples

The `examples/` directory contains 18 ready-to-use JSON files demonstrating all XGUI capabilities, from simple 2-3 widget interfaces to complex multi-tab applications:

- **Simple Examples** (01-05): Basic widgets and layouts
- **Medium Complexity** (06-10): Forms, tabs, calendars, image viewers
- **Complex Applications** (11-15): Data grids, tree views, menus, dashboards
- **Specialized Demos** (16-18): Layout techniques, radio buttons, complete widget showcase

See [examples/README.md](examples/README.md) for detailed documentation of each example.

### XGUI Example Viewer

Test and preview JSON GUI definitions instantly:

```bash
# Build the example viewer
make -f Makefile.gtk xgui-example  # or Makefile.osx / Makefile.gnustep

# Run with any JSON file
./output/xgui-example examples/01-simple-button.json
```

See [tests/xgui-example/README.md](tests/xgui-example/README.md) for complete documentation.

### C++ Examples

Check the `tests/` directory for additional C++ example applications demonstrating various XGUI features.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Links

- **GitHub Repository**: [https://github.com/fsoft72/xgui](https://github.com/fsoft72/xgui)
- **Documentation**: Available in the `docs/` directory

---

**Note**: XGUI is under active development. APIs may change between versions.

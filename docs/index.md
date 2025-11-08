# XGUI Documentation

Welcome to the XGUI documentation! XGUI is a cross-platform GUI library for C++ that provides a comprehensive set of widgets and tools for building desktop applications.

## Overview

XGUI offers:

- **Cross-platform support** - Write once, run on multiple platforms
- **Rich widget library** - Comprehensive set of UI components
- **Event-driven architecture** - Flexible callback system for handling user interactions
- **Customizable styling** - StyleManager for theming and appearance customization
- **Dynamic properties** - Runtime property configuration for all widgets

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

## Documentation Structure

- **[Getting Started](getting-started.md)** - Learn how to set up and use XGUI
- **[API Reference](api/index.md)** - Complete API documentation for all classes and widgets

## Main Object Hierarchy

```
Object
├── Widget
│   ├── Button
│   ├── Calendar
│   ├── Checkbox
│   ├── Container
│   │   ├── Frame
│   │   ├── HBox
│   │   ├── VBox
│   │   ├── Window
│   │   └── Tab
│   ├── Entry
│   ├── ImageView
│   ├── Label
│   ├── Menu
│   ├── Progressbar
│   ├── Slider
│   ├── Space
│   ├── Spin
│   ├── Toolbar
│   └── View
│       ├── Combobox
│       ├── List
│       └── Tree
├── Image
├── Master
├── TreeNode
│   └── Model
└── StyleManager
```

## Key Concepts

### Widgets

All UI components in XGUI derive from the `Widget` class, which itself derives from `Object`. Widgets provide the building blocks for your user interface.

### Containers

Containers are special widgets that can hold other widgets. They manage layout and organization of child widgets.

### Properties

All objects support a dynamic property system. Properties can be set and retrieved using the `set()` and `get()` methods.

### Events

Widgets support various events (onclick, onmousedown, onkeypress, etc.) that can be linked to callback functions for handling user interactions.

## License

See the LICENSE file in the project root for license information.

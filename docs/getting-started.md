# Getting Started with XGUI

This guide will help you get started with XGUI, a cross-platform GUI library for C++.

## Installation

### Building from Source

1. Clone the repository:
```bash
git clone https://github.com/fsoft72/xgui.git
cd xgui
```

2. Build the library:
```bash
make
```

3. Install (optional):
```bash
sudo make install
```

## Basic Concepts

### The Master Object

The `Master` object is the central controller for XGUI applications. It manages the main event loop and provides factory methods for creating widgets.

```cpp
xgui::Master *master = xgui::Master::Create();
```

### Creating Widgets

Widgets cannot be directly instantiated. Instead, use the factory methods provided by the `Master` class:

```cpp
xgui::Window *window = master->CreateWindow();
xgui::Button *button = master->CreateButton();
xgui::Entry *entry = master->CreateEntry();
```

### Working with Properties

All XGUI objects support dynamic properties. Use `set()` and `get()` methods to configure widgets:

```cpp
// Setting properties
button->set("text", "Click Me");
button->set("enabled", "1");

// Getting properties
std::string text;
button->get("text", text);
```

### Adding Widgets to Containers

Use the container's `add()` method to add child widgets:

```cpp
xgui::Window *window = master->CreateWindow();
xgui::Button *button = master->CreateButton();

window->add(button);
```

### Event Handling

Connect callbacks to widget events using the `linkEvent()` method:

```cpp
class MyCallback : public xgui::Callback {
public:
    virtual void call() {
        std::cout << "Button clicked!" << std::endl;
    }
};

MyCallback *callback = new MyCallback();
button->linkEvent("onclick", callback);
```

### Running the Application

Start the main event loop using the `Run()` method:

```cpp
master->Run();
```

## Complete Example

Here's a complete example that creates a simple window with a button:

```cpp
#include <xgui/xgui.h>
#include <iostream>

class ButtonCallback : public xgui::Callback {
public:
    virtual void call() {
        std::cout << "Button was clicked!" << std::endl;
    }
};

int main() {
    // Create the master object
    xgui::Master *master = xgui::Master::Create();

    // Create a window
    xgui::Window *window = master->CreateWindow();
    window->set("title", "My First XGUI App");
    window->set("width", "400");
    window->set("height", "300");

    // Create a vertical box for layout
    xgui::VBox *vbox = master->CreateVBox();
    vbox->set("margin", "10");
    vbox->set("spacing", "10");

    // Create a label
    xgui::Label *label = master->CreateLabel();
    label->set("text", "Welcome to XGUI!");

    // Create a button
    xgui::Button *button = master->CreateButton();
    button->set("text", "Click Me!");

    // Connect button callback
    ButtonCallback *callback = new ButtonCallback();
    button->linkEvent("onclick", callback);

    // Add widgets to container
    vbox->add(label);
    vbox->add(button);
    window->add(vbox);

    // Show window
    window->set("visible", "1");

    // Run the application
    master->Run();

    return 0;
}
```

## Next Steps

- Explore the [API Reference](api/index.md) to learn about all available widgets and their properties
- Check out specific widget documentation in the [Widgets](api/widgets/button.md) section
- Learn about [Containers](api/containers/container.md) for managing layout
- Understand the [Callback](api/callbacks/index.md) system for event handling

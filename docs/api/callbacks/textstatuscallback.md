# TextStatusCallback

The `TextStatusCallback` class is an interface for callback functions that accept a text parameter and an integer status parameter. It derives from `Callback` and is used for events that provide both string and integer data.

## Class Hierarchy

```
Callback
└── TextStatusCallback
```

## Description

TextStatusCallback is used for events that need to pass both text information and a status code to the event handler, such as menu selections, toolbar button clicks, or item selections with state information.

## Methods

### `int call(Widget * w, std::string const &text, int status)`

Calls the method wrapped by this callback object. The method accepts the widget reference, a text parameter, and an integer parameter.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event
- `text` - String parameter containing event-specific text data
- `status` - Integer parameter containing event-specific status information

**Returns:**
- `xgui::EVT_PROPAGATE` - The event is propagated to the next event handlers
- `xgui::EVT_BLOCK` - The event is not propagated

---

# CppFTextStatusCallback

The `CppFTextStatusCallback` class is a standard C++ wrapper for callbacks with a text and an integer parameter. It derives from `TextStatusCallback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── TextStatusCallback
    └── CppFTextStatusCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI text-status callbacks, making it easy to handle events that provide both text and status information.

## Constructor

### `CppFTextStatusCallback(int(*fptr)(Widget *, std::string const &, int status))`

Initializes the CppFTextStatusCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*, std::string const &, int)`

---

## Example

```cpp
#include <xgui/xgui.h>

int onMenuSelect(xgui::Widget* widget, std::string const &itemId, int checked) {
    std::cout << "Menu item '" << itemId << "' selected";
    if (checked) {
        std::cout << " (checked)";
    }
    std::cout << std::endl;
    return xgui::EVT_PROPAGATE;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Menu* menu = master->CreateMenu(window);

    menu->addItem("", "file", "File");
    menu->addItem("file", "save", "Save");

    // Use CppFTextStatusCallback to wrap the function pointer
    xgui::CppFTextStatusCallback* callback =
        new xgui::CppFTextStatusCallback(onMenuSelect);
    menu->linkEvent("onselect", callback);

    window->show();
    return master->Run();
}
```

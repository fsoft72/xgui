# TextCallback

The `TextCallback` class is an interface for callback functions that accept a text parameter. It derives from `Callback` and is used for events that provide string data.

## Class Hierarchy

```
Callback
└── TextCallback
```

## Description

TextCallback is used for events that need to pass text information to the event handler, such as text changes in entry widgets, key presses, or item selections.

## Methods

### `int call(Widget * w, std::string const &text)`

Calls the method wrapped by this callback object. The method accepts the widget reference and a text parameter.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event
- `text` - String parameter containing event-specific text data

**Returns:**
- `xgui::EVT_PROPAGATE` - The event is propagated to the next event handlers
- `xgui::EVT_BLOCK` - The event is not propagated

---

# CppFTextCallback

The `CppFTextCallback` class is a standard C++ wrapper for callbacks with a text parameter. It derives from `TextCallback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── TextCallback
    └── CppFTextCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI text callbacks, making it easy to handle events that provide text information.

## Constructor

### `CppFTextCallback(int(*fptr)(Widget *, std::string const &))`

Initializes the CppFTextCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*, std::string const &)`

---

## Example

```cpp
#include <xgui/xgui.h>

int onTextChange(xgui::Widget* widget, std::string const &text) {
    std::cout << "Text changed to: " << text << std::endl;
    return xgui::EVT_PROPAGATE;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Entry* entry = master->CreateEntry(window, "");

    // Use CppFTextCallback to wrap the function pointer
    xgui::CppFTextCallback* callback = new xgui::CppFTextCallback(onTextChange);
    entry->linkEvent("onchange", callback);

    window->show();
    return master->Run();
}
```

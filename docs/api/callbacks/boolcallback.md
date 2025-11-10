# BoolCallback

The `BoolCallback` class is an interface for callback functions that accept a boolean parameter. It derives from `Callback` and is used for events that provide boolean state information.

## Class Hierarchy

```
Callback
└── BoolCallback
```

## Description

BoolCallback is used for events that need to pass boolean state information to the event handler, such as checkbox state changes or toggle events.

## Methods

### `int call(Widget * w, bool value)`

Calls the method wrapped by this callback object. The method accepts the widget reference and a boolean parameter.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event
- `value` - Boolean parameter indicating the state (true/false, checked/unchecked, on/off)

**Returns:**
- `xgui::EVT_PROPAGATE` - The event is propagated to the next event handlers
- `xgui::EVT_BLOCK` - The event is not propagated

---

# CppFBoolCallback

The `CppFBoolCallback` class is a standard C++ wrapper for callbacks with a boolean parameter. It derives from `BoolCallback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── BoolCallback
    └── CppFBoolCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI boolean callbacks, making it easy to handle events that provide boolean state information.

## Constructor

### `CppFBoolCallback(int(*fptr)(Widget *, bool))`

Initializes the CppFBoolCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*, bool)`

---

## Example

```cpp
#include <xgui/xgui.h>

int onCheckboxChange(xgui::Widget* widget, bool checked) {
    std::cout << "Checkbox is now: " << (checked ? "checked" : "unchecked") << std::endl;
    return xgui::EVT_PROPAGATE;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Checkbox* checkbox = master->CreateCheckbox(window, "Enable Option", false);

    // Use CppFBoolCallback to wrap the function pointer
    xgui::CppFBoolCallback* callback = new xgui::CppFBoolCallback(onCheckboxChange);
    checkbox->linkEvent("onchange", callback);

    window->show();
    return master->Run();
}
```

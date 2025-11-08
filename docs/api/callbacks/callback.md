# Callback

The `Callback` class is the basic interface for all callback types in XGUI. It provides the foundation for event handling throughout the framework.

## Class Hierarchy

```
Callback
```

## Description

Callbacks are used to link custom event handler code to widget events. This is the base class for all callback types, providing a simple interface that accepts a widget reference.

## Methods

### `int call(Widget * w)`

Calls the method wrapped by this callback object. The method accepts the widget reference as the only parameter.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event

**Returns:**
- `xgui::EVT_PROPAGATE` - The event is propagated to the next event handlers
- `xgui::EVT_BLOCK` - The event is not propagated

---

# CppFCallback

The `CppFCallback` class is a standard C++ wrapper for basic callbacks. It derives from `Callback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── CppFCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI callbacks, making it easy to integrate callback functionality without creating custom callback classes.

## Constructor

### `CppFCallback(int(*fptr)(Widget*))`

Initializes the CppFCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*)`

---

## Example

```cpp
#include <xgui/xgui.h>

int onButtonClick(xgui::Widget* widget) {
    std::cout << "Button clicked!" << std::endl;
    return xgui::EVT_PROPAGATE;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Button* button = master->CreateButton(window, "Click Me");

    // Use CppFCallback to wrap the function pointer
    xgui::CppFCallback* callback = new xgui::CppFCallback(onButtonClick);
    button->linkEvent("onclick", callback);

    window->show();
    return master->Run();
}
```

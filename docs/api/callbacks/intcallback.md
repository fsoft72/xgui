# IntCallback

The `IntCallback` class is an interface for callback functions that accept an integer parameter. It derives from `Callback` and is used for events that provide numeric data.

## Class Hierarchy

```
Callback
└── IntCallback
```

## Description

IntCallback is used for events that need to pass integer information to the event handler, such as slider value changes, tab page selections, or numeric input.

## Methods

### `int call(Widget * w, int value)`

Calls the method wrapped by this callback object. The method accepts the widget reference and an integer parameter.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event
- `value` - Integer parameter containing event-specific numeric data

**Returns:**
- `xgui::EVT_PROPAGATE` - The event is propagated to the next event handlers
- `xgui::EVT_BLOCK` - The event is not propagated

---

# CppFIntCallback

The `CppFIntCallback` class is a standard C++ wrapper for callbacks with an integer parameter. It derives from `IntCallback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── IntCallback
    └── CppFIntCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI integer callbacks, making it easy to handle events that provide numeric information.

## Constructor

### `CppFIntCallback(int(*fptr)(Widget *, int))`

Initializes the CppFIntCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*, int)`

---

## Example

```cpp
#include <xgui/xgui.h>

int onSliderChange(xgui::Widget* widget, int value) {
    std::cout << "Slider value changed to: " << value << std::endl;
    return xgui::EVT_PROPAGATE;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Slider* slider = master->CreateSlider(window, 0, 100, false);

    // Use CppFIntCallback to wrap the function pointer
    xgui::CppFIntCallback* callback = new xgui::CppFIntCallback(onSliderChange);
    slider->linkEvent("oninput", callback);

    window->show();
    return master->Run();
}
```

# TextPairCallback

The `TextPairCallback` class is an interface for callback functions that accept two string parameters. It derives from `Callback` and is used for events that provide two text values.

## Class Hierarchy

```
Callback
└── TextPairCallback
```

## Description

TextPairCallback is used for events that need to pass two text values to the event handler, such as comparison functions for sorting operations in List and Tree widgets.

## Methods

### `int call(Widget * w, std::string const &text1, std::string const &text2)`

Calls the method wrapped by this callback object. The method accepts the widget reference and two string parameters.

**Parameters:**
- `w` - Pointer to the Widget that triggered the event
- `text1` - First string parameter
- `text2` - Second string parameter

**Returns:**
- Typically returns a comparison result (negative, zero, or positive) when used for sorting
- Can also return `xgui::EVT_PROPAGATE` or `xgui::EVT_BLOCK` for event propagation control

---

# CppFTextPairCallback

The `CppFTextPairCallback` class is a standard C++ wrapper for callbacks with two string parameters. It derives from `TextPairCallback` and provides a convenient way to use function pointers as callbacks.

## Class Hierarchy

```
Callback
└── TextPairCallback
    └── CppFTextPairCallback
```

## Description

This wrapper class allows you to use standard C++ function pointers as XGUI text-pair callbacks, making it easy to implement comparison or pair-processing functionality.

## Constructor

### `CppFTextPairCallback(int(*fptr)(Widget *, std::string const &text1, std::string const &text2))`

Initializes the CppFTextPairCallback class with the specified function pointer.

**Parameters:**
- `fptr` - Function pointer that matches the signature `int function(Widget*, std::string const &, std::string const &)`

---

## Example

```cpp
#include <xgui/xgui.h>

int compareItems(xgui::Widget* widget,
                 std::string const &item1,
                 std::string const &item2) {
    // Simple alphabetical comparison
    if (item1 < item2) return -1;
    if (item1 > item2) return 1;
    return 0;
}

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();

    // Create a list widget
    xgui::List* list = master->CreateList(window);
    list->addColumn(0, "name", "Name", 200);

    // Create and populate a model
    xgui::Model* model = master->CreateModel();
    model->appendChild("Zebra");
    model->appendChild("Apple");
    model->appendChild("Mango");
    list->setModel(model);

    // Use CppFTextPairCallback for sorting
    xgui::CppFTextPairCallback* callback =
        new xgui::CppFTextPairCallback(compareItems);
    list->sort("name", callback);

    window->show();
    return master->Run();
}
```

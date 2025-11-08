# Callback System

The XGUI callback system provides a flexible mechanism for handling events in your application. Callbacks are used to link custom event handler code to widget events, allowing you to respond to user interactions and system events.

## Overview

XGUI provides several callback types, each designed for different event scenarios:

- **[Callback](callback.md)** - Basic callback interface for simple events
- **[TextCallback](textcallback.md)** - Callback with a text parameter
- **[TextStatusCallback](textstatuscallback.md)** - Callback with text and integer status parameters
- **[TextPairCallback](textpaircallback.md)** - Callback with two text parameters

## Callback Types

### Callback

The base callback type accepts only a widget reference. It's used for simple events like button clicks, focus changes, and window closures.

**Common Events:**
- `onclick` - Widget clicked
- `onfocus` - Widget gained focus
- `onblur` - Widget lost focus
- `ondestroy` - Widget is being destroyed

### TextCallback

This callback type accepts a widget reference and a text parameter. It's used when events need to provide string data.

**Common Events:**
- `onchange` - Text has changed (Entry, Spin)
- `onsubmit` - Enter key pressed (Entry, Spin, Combobox)
- `onkeypress` - Key pressed and released
- `onmousemove` - Mouse moved (provides position)

### TextStatusCallback

This callback type accepts a widget reference, a text parameter, and an integer status parameter. It's used when events provide both text and state information.

**Common Events:**
- `onselect` - Item selected (Menu, List, Tree, Combobox)
- `oncheck` - Item checked/unchecked (List, Tree)
- `onmousedown` - Mouse button pressed
- `onmouseup` - Mouse button released
- `onmouseclick` - Mouse clicked

### TextPairCallback

This callback type accepts a widget reference and two text parameters. It's primarily used for comparison operations.

**Common Use Cases:**
- Sorting list items
- Sorting tree nodes
- Custom comparison operations

## Using Callbacks

### Method 1: Derive from Callback Class

```cpp
#include <xgui/xgui.h>

class MyClickHandler : public xgui::Callback {
public:
    virtual int call(xgui::Widget* widget) {
        std::cout << "Button clicked!" << std::endl;
        return xgui::EVT_PROPAGATE;
    }
};

int main() {
    xgui::Master* master = xgui::Master::Instance();
    xgui::Window* window = master->CreateWindow();
    xgui::Button* button = master->CreateButton(window, "Click Me");

    MyClickHandler* handler = new MyClickHandler();
    button->linkEvent("onclick", handler);

    window->show();
    return master->Run();
}
```

### Method 2: Use C++ Function Pointer Wrappers

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

    xgui::CppFCallback* callback = new xgui::CppFCallback(onButtonClick);
    button->linkEvent("onclick", callback);

    window->show();
    return master->Run();
}
```

## Return Values

Callback functions should return one of the following values:

- `xgui::EVT_PROPAGATE` - Allow the event to propagate to other handlers
- `xgui::EVT_BLOCK` - Block the event from propagating (also prevents default behavior in some cases)

For comparison callbacks (TextPairCallback), return values typically indicate:
- Negative value - First item is less than second item
- Zero - Items are equal
- Positive value - First item is greater than second item

## Linking Events

To link a callback to a widget event, use the `linkEvent()` method:

```cpp
widget->linkEvent("event_name", callback_object);
```

Common event names are documented in each widget's Events section.

## Best Practices

1. **Memory Management**: Callbacks are typically allocated with `new` and should be deleted when no longer needed, or managed with smart pointers.

2. **Event Propagation**: Return `EVT_PROPAGATE` unless you specifically need to block event propagation or default behavior.

3. **Blocking Events**: Some events (like `onchange`, `onclose`) can be blocked by returning `EVT_BLOCK`, which prevents the default action from occurring.

4. **Type Safety**: Always use the correct callback type for each event. Check the widget documentation for the required callback type.

5. **Widget Access**: The widget parameter passed to callbacks can be cast to the specific widget type if needed:
   ```cpp
   xgui::Button* button = dynamic_cast<xgui::Button*>(widget);
   ```

## See Also

- [Widget Events](../core/widget.md#events)
- [Object Class](../core/object.md)
- [Event Handling Guide](../../guide/events.md) (if available)

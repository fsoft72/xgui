# Widget

The `Widget` class is the base class for all UI components in XGUI. It derives from `Object` and provides fundamental functionality for event handling, layout management, and user interaction.

## Class Hierarchy

```
Object
└── Widget
```

## Methods

### Widget Information

#### `xguimpl::Widget * getImpl()`

Returns a pointer to the widget implementation class.

**Returns:** Pointer to the platform-specific widget implementation.

---

#### `xgui::Container * getParent()`

Returns the widget's parent container.

**Returns:** Pointer to the parent Container, or `NULL` if the widget has no parent.

---

#### `xgui::Window * getContext()`

Returns the widget context (i.e., the top-level window that contains it).

**Returns:** Pointer to the containing Window.

---

#### `std::string getPath()`

Returns a string representing the widget and all its parents up to the containing top-level window.

The returned string has the following format:
```
<top_level_window_id>/<parent_id>[/<parent_id> ...]/<widget_id>
```

**Returns:** The full path of the widget.

---

### Layout and Display

#### `void setAsFloating()`

Unlinks the widget from its parent, making it a floating widget.

---

#### `void recalcLayout()`

Recalculates the dimensions and positions of the widget and all its children.

---

#### `void redraw()`

Forces a redraw of the widget.

---

### Event Handling

#### `bool linkEvent(std::string const &name, xgui::Callback * cb)`

Links a callback method to the specified event `name`.

**Parameters:**
- `name` - The event name (e.g., "onclick", "onmousedown")
- `cb` - Pointer to the Callback object to link

**Returns:** `false` if the event could not be found, `true` otherwise.

---

#### `xgui::Callback * getEvent(std::string const &name)`

Returns a callback linked to the specified event `name`.

**Parameters:**
- `name` - The event name

**Returns:** Pointer to the linked Callback, or `NULL` if not linked.

---

#### `bool hasEvent(std::string const &name)`

Returns true if the specified event `name` is linked.

**Parameters:**
- `name` - The event name

**Returns:** `true` if the event is linked, `false` otherwise.

---

### Mouse Interaction

#### `std::pair<int, int> getMousePos()`

Returns an `<x, y>` pair that specifies the mouse position relative to the widget's upper left corner.

**Returns:** Pair of integers representing mouse coordinates.

---

## Properties

### `enabled` (Read/Write)

Enables or disables mouse and keyboard input to the widget.

**Values:**
- `"1"` - Widget is enabled (accepts input)
- `"0"` - Widget is disabled (ignores input)

**Default:** `"1"`

---

### `tooltip-text` (Read/Write)

Sets or retrieves the text of the widget tooltip.

**Values:**
- String - The tooltip text
- `""` - No tooltip will be shown (empty string)

**Default:** `""`

---

### `visible` (Read/Write)

Controls the visibility of the widget.

**Values:**
- `"1"` - Widget is visible
- `"0"` - Widget is hidden

**Default:** `"1"`

---

### `focused` (Read/Write)

Sets or removes the keyboard focus to the widget.

**Values:**
- `"1"` - Widget has keyboard focus
- `"0"` - Widget does not have focus

---

### `width` (Read-Only)

Gets the widget width in pixels.

**Type:** Integer (as string)

---

### `height` (Read-Only)

Gets the widget height in pixels.

**Type:** Integer (as string)

---

### `path` (Read-Only)

Retrieves the path of the widget (the same returned by the `getPath()` method).

**Type:** String

---

## Events

All events can be linked using the `linkEvent()` method. The event name is specified as a string, and different events accept different callback types.

### Lifecycle Events

#### `ondestroy` (xgui::Callback)

Fired when the widget is destroyed.

---

#### `onfocus` (xgui::Callback)

Fired when the widget gets keyboard focus.

---

#### `onblur` (xgui::Callback)

Fired when the widget loses keyboard focus.

---

### Mouse Events

#### `onmousedown` (xgui::TextStatusCallback)

Fired when the user presses a mouse button while the cursor is in the client area of the widget.

**Parameters:**
- `text` - Specifies which button was clicked: `"left"`, `"right"`, or `"middle"`
- `status` - Bitmask of pressed keys/buttons:
  - `xgui::mouseLeftMask` - Left mouse button is pressed
  - `xgui::mouseMiddleMask` - Middle mouse button is pressed
  - `xgui::mouseRightMask` - Right mouse button is pressed
  - `xgui::keyControlMask` - Control key is pressed
  - `xgui::keyShiftMask` - Shift key is pressed
  - `xgui::keyAltMask` - Alt key is pressed

---

#### `onmouseup` (xgui::TextStatusCallback)

Fired when the user releases a mouse button while the cursor is in the client area of the widget.

**Parameters:**
- `text` - Specifies which button was released: `"left"`, `"right"`, or `"middle"`
- `status` - Same as `onmousedown`

---

#### `onmouseclick` (xgui::TextStatusCallback)

Fired when the user clicks (i.e., presses and releases) a mouse button while the cursor is in the client area of the widget.

**Parameters:**
- `text` - Specifies which button was clicked: `"left"`, `"right"`, or `"middle"`
- `status` - Same as `onmousedown`

---

#### `onmousedblclick` (xgui::TextStatusCallback)

Fired when the user double-clicks a mouse button while the cursor is in the client area of the widget.

**Parameters:**
- `text` - Specifies which button was double-clicked: `"left"`, `"right"`, or `"middle"`
- `status` - Same as `onmousedown`

---

#### `onmousemove` (xgui::TextCallback)

Fired when the user moves the cursor over the client area of the widget.

**Parameters:**
- `text` - Cursor position relative to widget's upper left corner in format `"x y"`

---

#### `onmouseover` (xgui::Callback)

Fired when the cursor enters the client area of the widget.

---

#### `onmouseout` (xgui::Callback)

Fired when the cursor leaves the client area of the widget.

---

### Keyboard Events

#### `onkeydown` (xgui::TextCallback)

Fired when the user presses a key while the widget has keyboard focus.

**Parameters:**
- `text` - The pressed key

---

#### `onkeyup` (xgui::TextCallback)

Fired when the user releases a key while the widget has keyboard focus.

**Parameters:**
- `text` - The released key

---

#### `onkeypress` (xgui::TextCallback)

Fired when the user presses and releases a key while the widget has keyboard focus.

**Parameters:**
- `text` - The released key

---

### State Change Events

#### `onshow` (xgui::Callback)

Fired when the widget becomes visible.

---

#### `onhide` (xgui::Callback)

Fired when the widget becomes hidden.

---

#### `onenable` (xgui::Callback)

Fired when the widget becomes enabled (i.e., it accepts mouse and keyboard input).

---

#### `ondisable` (xgui::Callback)

Fired when the widget becomes disabled (i.e., it ignores mouse and keyboard input).

---

### Geometry Events

#### `onresize` (xgui::TextCallback)

Fired when the widget size has changed.

**Parameters:**
- `text` - New size in format `"width height"`

---

#### `onmove` (xgui::TextCallback)

Fired when the widget position has changed.

**Parameters:**
- `text` - New position of upper-left corner in format `"x y"` (relative to parent)

---

## Example

```cpp
#include <xgui/xgui.h>

class MyClickCallback : public xgui::Callback {
public:
    virtual void call() {
        std::cout << "Widget clicked!" << std::endl;
    }
};

int main() {
    xgui::Master *master = xgui::Master::Create();
    xgui::Window *window = master->CreateWindow();
    xgui::Button *button = master->CreateButton();

    // Set properties
    button->set("text", "Click Me");
    button->set("enabled", "1");
    button->set("tooltip-text", "This is a button");

    // Link event
    MyClickCallback *callback = new MyClickCallback();
    button->linkEvent("onclick", callback);

    window->add(button);
    window->set("visible", "1");

    master->Run();
    return 0;
}
```

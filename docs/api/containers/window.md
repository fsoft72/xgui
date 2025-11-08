# Window

The `Window` widget represents a top-level window in the application. It derives from `Container` and provides functionality for window management including modality, positioning, sizing, and window state control.

## Class Hierarchy

```
Object
└── Widget
    └── Container
        └── Window
```

## Methods

### Window Display

#### `void show()`

Shows the window and all of its children.

---

#### `int modalRun()`

Shows the window as a modal window (i.e., this method doesn't return until the window is destroyed). The return value is the value passed to the `modalReturn()` method.

**Returns:** The response value set by `modalReturn()`.

---

#### `bool isModal()`

Returns true if the window is modal; false otherwise.

**Returns:** `true` if modal, `false` otherwise.

---

#### `void modalReturn(int response)`

Exits from a modal loop, closes the related window, and returns the `response` value as the return value of the `modalRun()` method.

**Parameters:**
- `response` - The value to return from `modalRun()`

---

### Child Access

#### `xgui::Widget * getChild()`

Returns the window child. Same as `getChild(0)`.

**Returns:** Pointer to the window's child Widget.

---

#### `xgui::Menu * getMenu()`

Returns the menu assigned to the window, if one exists.

**Returns:** Pointer to the Menu, or `NULL` if no menu is assigned.

---

### Window State

#### `void maximize()`

Maximizes the window.

---

#### `void minimize()`

Minimizes the specified window and activates the next top-level window in the z-order.

---

#### `void restore()`

Activates and displays the window. If the window is minimized or maximized, it will be restored to its original size and position.

---

### Window Icon

#### `void setIcon(Image * img)`

Sets the image displayed in the window title bar.

**Parameters:**
- `img` - Pointer to the Image to display

---

#### `xgui::Image * getIcon()`

Returns the image displayed in the window title bar, or NULL if no image is displayed.

**Returns:** Pointer to the Image, or `NULL` if no icon is set.

---

## Properties

### `text` (Read/Write)

Sets or retrieves the text displayed on the window title bar.

**Type:** String

**Default:** `""`

---

### `size` (Read/Write)

Sets or retrieves the window dimensions.

**Type:** String

**Format:** `"width height"`

---

### `position` (Read/Write)

Sets or retrieves the window position on the screen.

**Type:** String

**Format:** `"x y"` or `"center"` to position at screen center

---

### `resizable` (Read/Write)

Controls whether the window can be resized.

**Values:**
- `"1"` - Window has a sizing border
- `"0"` - Window can't be resized

**Default:** `"1"`

---

### `border` (Read/Write)

Controls whether the window has a border and title bar.

**Values:**
- `"1"` - Window has a border and title bar
- `"0"` - Window doesn't have any border or title bar

**Default:** `"1"`

---

### `status-text` (Read/Write)

Controls the status bar at the bottom of the window.

**Type:** String

**Values:**
- Non-empty string - Status bar is displayed with the specified text
- Empty string - No status bar is displayed

**Default:** `""`

---

### `icon` (Read/Write)

Sets or retrieves the path of the file of the image displayed on the window title bar.

**Type:** String

**See also:** `setIcon()` method.

---

## Events

### `onclose` (xgui::Callback)

Fired when the window is about to be closed.

**Note:** Returning `xgui::EVT_BLOCK` from the callback function will prevent the window from closing.

---

## Pushed Properties

This widget does not register any properties on its children.

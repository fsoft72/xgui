# Toolbar

The `Toolbar` widget provides a toolbar with buttons that can display icons, text, or both. It derives from `Widget` and supports standard buttons, checkable buttons, radio button groups, and separators.

## Class Hierarchy

```
Object
└── Widget
    └── Toolbar
```

## Methods

### Toolbar Item Management

#### `int addItem(std::string const &id, std::string const &text, xgui::Image * icon, std::string const &tooltip, std::string const & type = "")`

Adds a new toolbar item.

**Parameters:**
- `id` - ID of the element to be added
- `text` - Text displayed on the item when the toolbar has a style that displays text
- `icon` - Pointer to Image displayed when the toolbar has a style that displays images
- `tooltip` - Tooltip text to show for the item
- `type` - Type of toolbar item (default: `""`):
  - `""` - Standard toolbar button
  - `"separator"` - Creates an empty space
  - `"check"` - Creates a checkable button
  - `"radio_begin"` - Pseudo-item that specifies the beginning of a radio group
  - `"radio_end"` - Pseudo-item that specifies the end of a radio group

**Returns:** Integer status code.

---

#### `void delItem(const std::string & name)`

Removes the specified item from the toolbar.

**Parameters:**
- `name` - Name of the item to remove

---

### Button State Management

#### `void enableButton(const std::string & name)`

Enables the specified toolbar button (the button can be clicked).

**Parameters:**
- `name` - Name of the button to enable

---

#### `void disableButton(const std::string & name)`

Disables the specified toolbar button (the button can't be clicked and is grayed).

**Parameters:**
- `name` - Name of the button to disable

---

#### `void checkButton(const std::string & name)`

Puts the specified checkable toolbar button in the "checked" state.

**Parameters:**
- `name` - Name of the button to check

---

#### `void uncheckButton(const std::string & name)`

Puts the specified checkable toolbar button in the "unchecked" state.

**Parameters:**
- `name` - Name of the button to uncheck

---

#### `bool isButtonChecked(const std::string & name)`

Returns true if the specified checkable toolbar button is in the "checked" state.

**Parameters:**
- `name` - Name of the button to check

**Returns:** `true` if checked, `false` otherwise.

---

## Properties

### `style` (Read/Write)

Sets or retrieves the toolbar style.

**Values:**
- `"icons"` - Only icons are displayed on toolbar buttons (no text is displayed; if a button does not have a tooltip text, then the text is displayed as a tooltip)
- `"text"` - Only text is displayed on toolbar buttons (no image is displayed)
- `"both"` - Both icons and text are displayed on toolbar buttons

**Default:** `"both"`

---

## Events

### `onclick` (xgui::TextStatusCallback)

Fired when the user clicks on a toolbar button.

**Parameters:**
- `text` - Name of the clicked toolbar button
- `status` - Set to `1` if the clicked button has been checked, otherwise `0`

---

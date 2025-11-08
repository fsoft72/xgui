# Menu

The `Menu` widget provides a hierarchical menu system with support for standard items, separators, checkable items, and radio groups. It derives from `Widget` and offers extensive functionality for creating and managing menu structures.

## Class Hierarchy

```
Object
└── Widget
    └── Menu
```

## Methods

### Menu Item Management

#### `void addItem(std::string const &parent_id, std::string const &id, std::string const &text, xgui::Image * icon = 0, std::string const & type = "")`

Adds a new menu item.

**Parameters:**
- `parent_id` - ID of the parent menu item, or empty string to add at root level
- `id` - Identification string assigned to the new menu item
- `text` - The displayed menu item text
- `icon` - Optional pointer to an Image displayed near the menu item text (default: `0`)
- `type` - Type of menu item (default: `""`):
  - `""` - Standard menu item
  - `"separator"` - Creates a line separator
  - `"check"` - Creates a checkable menu item (with check state icon)
  - `"radio_begin"` - Pseudo-item (not visible) marking the beginning of a radio group
  - `"radio_end"` - Pseudo-item (not visible) marking the end of a radio group

---

#### `void delItem(std::string const &id)`

Removes the specified menu item.

**Parameters:**
- `id` - ID of the menu item to remove

---

#### `void popupMenu(std::string const &id)`

Displays the popup menu identified by `id` at the current mouse position.

**Parameters:**
- `id` - ID of the menu to display as a popup

---

### Menu Item State

#### `void enableItem(std::string const &id)`

Enables the specified menu item (the item can be clicked or selected).

**Parameters:**
- `id` - ID of the menu item to enable

---

#### `void disableItem(std::string const &id)`

Disables the specified menu item (the item can't be clicked or selected and is displayed in gray).

**Parameters:**
- `id` - ID of the menu item to disable

---

#### `bool isItemEnabled(std::string const &id)`

Returns true if the specified item is enabled, false otherwise.

**Parameters:**
- `id` - ID of the menu item to check

**Returns:** `true` if enabled, `false` otherwise.

---

#### `void checkItem(std::string const &id)`

Marks the specified checkable menu item as checked.

**Parameters:**
- `id` - ID of the menu item to check

---

#### `void uncheckItem(std::string const &id)`

Removes a check from the checkable menu item.

**Parameters:**
- `id` - ID of the menu item to uncheck

---

#### `bool isItemChecked(std::string const &id)`

Returns true if the specified menu item is checked, false otherwise.

**Parameters:**
- `id` - ID of the menu item to check

**Returns:** `true` if checked, `false` otherwise.

---

### Menu Item Text

#### `void setItemText(std::string const &id, std::string const &text)`

Changes the text of the specified menu item.

**Parameters:**
- `id` - ID of the menu item
- `text` - New text for the menu item

---

#### `std::string getItemText(std::string const &id)`

Returns the text of the specified menu item.

**Parameters:**
- `id` - ID of the menu item

**Returns:** The menu item text.

---

## Properties

This widget has no additional properties beyond those inherited from `Widget`.

## Events

### `onselect` (xgui::TextStatusCallback)

Fired when the user clicks on a menu item.

**Parameters:**
- `text` - ID of the clicked menu item
- `status` - Set to `1` if the clicked menu item has been checked, otherwise `0`

---

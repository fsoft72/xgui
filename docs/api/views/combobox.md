# Combobox

The `Combobox` widget combines a text entry field with a drop-down list. It derives from `View` and allows users to either select from a list or optionally type their own value.

## Class Hierarchy

```
Object
└── Widget
    └── View
        └── Combobox
```

## Methods

### Text Manipulation

#### `void appendText(const std::string & text)`

Appends a text string to the end of the entry text.

**Parameters:**
- `text` - The text to append

---

#### `void prependText(const std::string & text)`

Inserts a text string at the beginning of the entry text.

**Parameters:**
- `text` - The text to prepend

---

#### `void insertText(const std::string & text, int pos)`

Inserts a text string at the specified position in the entry text.

**Parameters:**
- `text` - The text to insert
- `pos` - The position at which to insert the text

---

### Dropdown Management

#### `void popup()`

Shows the drop-down list associated with the combobox.

---

#### `void popdown()`

Hides the drop-down list associated with the combobox.

---

### Editable State

#### `bool isEditable()`

Returns true if the combobox entry part can be edited (i.e., the text can be manually inserted); false otherwise.

**Returns:** `true` if editable, `false` otherwise.

---

#### `void setAsEditable(bool editable)`

Controls whether the combobox entry part can be manually edited.

**Parameters:**
- `editable` - If `true`, the entry part can be manually edited; otherwise, text can only be changed by choosing an item from the list part

---

## Properties

### `text` (Read/Write)

Sets or retrieves the text displayed in the entry part of the combobox.

**Type:** String

**Default:** `""`

---

### `selected-item` (Read/Write)

Sets or retrieves the currently selected item of the list part of the combobox.

**Type:** Integer (as string)

**Values:** Zero-based index of the item

---

### `maxlength` (Read/Write)

Sets or retrieves the maximum length of the entry part text.

**Type:** Integer (as string)

**Default:** `0` (unlimited)

---

### `editable` (Read/Write)

Controls whether the combobox entry part can be manually edited.

**Values:**
- `"1"` - Entry part can be manually edited
- `"0"` - Text can only be changed by choosing an item from the list part

**Default:** `"0"`

---

## Events

### `onsubmit` (xgui::TextCallback)

Fired when the Enter key is pressed while the combobox widget has keyboard focus.

**Parameters:**
- `text` - The text of the entry part

---

### `onchange` (xgui::TextCallback)

Fired when the user has taken an action that may have altered text in the entry part of the combobox.

**Parameters:**
- `text` - The altered entry text

**Note:** Returning `xgui::EVT_BLOCK` from the callback function prevents the text from changing.

---

### `onselect` (xgui::TextStatusCallback)

Fired when the user changes the selected item of the list part of the combobox.

**Parameters:**
- `text` - The text of the newly selected item
- `status` - The zero-based index of the selected item

---

# Checkbox

The `Checkbox` widget provides a toggleable checkbox control with an associated text label. It derives from `Widget` and allows users to select or deselect options.

## Class Hierarchy

```
Object
└── Widget
    └── Checkbox
```

## Methods

### Value Management

#### `void setValue(std::string const &value)`

Assigns a value to the checkbox. This value will be passed as a parameter to the `onclick` event callback when the checkbox is checked.

**Parameters:**
- `value` - The value to assign to the checkbox

---

#### `std::string const &getValue()`

Returns the value assigned to the checkbox.

**Returns:** The checkbox value as a string.

---

## Properties

### `text` (Read/Write)

Sets or retrieves the text displayed near the checkbox.

**Type:** String

**Default:** `""`

---

### `alignment` (Read/Write)

Sets or retrieves the alignment of the checkbox text relative to the checkbox itself.

**Values:**
- `"left"` - Text is displayed to the left of the checkbox
- `"right"` - Text is displayed to the right of the checkbox

**Default:** `"right"`

---

### `value` (Read/Write)

Sets or retrieves the value assigned to the checkbox. This value will be passed as a parameter to the `onclick` event callback when the checkbox is checked.

**Type:** String

**Default:** `"on"`

---

### `checked` (Read/Write)

Controls the checked state of the checkbox.

**Values:**
- `"1"` - Checkbox is checked
- `"0"` - Checkbox is unchecked

**Default:** `"0"`

---

## Events

### `onclick` (xgui::TextCallback)

Fired when the checkbox is clicked.

**Parameters:**
- `text` - Equal to the value assigned to the checkbox (see `value` property or `setValue()` method) if the click changes the state to checked; otherwise the string parameter is empty

---

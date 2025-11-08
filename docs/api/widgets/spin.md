# Spin

The `Spin` widget provides a spinbox control for selecting numeric values using increment/decrement buttons or direct text input. It derives from `Widget` and allows users to select values within a specified range.

## Class Hierarchy

```
Object
└── Widget
    └── Spin
```

## Methods

This widget has no additional methods beyond those inherited from `Widget`.

## Properties

### `value` (Read/Write)

Sets or retrieves the current spin box value.

**Type:** Integer (as string)

---

### `min` (Read/Write)

Sets or retrieves the minimum spin box value.

**Type:** Integer (as string)

**Default:** `"0"`

---

### `max` (Read/Write)

Sets or retrieves the maximum spin box value.

**Type:** Integer (as string)

**Default:** `"100"`

---

## Events

### `onchange` (xgui::TextCallback)

Fired when the spin box value changes.

**Parameters:**
- `text` - The new spin box value

**Note:** Returning `xgui::EVT_BLOCK` from the callback function prevents the value from changing.

---

### `onsubmit` (xgui::TextCallback)

Fired when the Enter key is pressed while the spin widget has keyboard focus.

**Parameters:**
- `text` - The spin box value

---

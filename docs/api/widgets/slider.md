# Slider

The `Slider` widget provides a draggable slider control for selecting a value within a range. It derives from `Widget` and can be oriented horizontally or vertically.

## Class Hierarchy

```
Object
└── Widget
    └── Slider
```

## Methods

### Position Management

#### `int getPos()`

Returns the current position of the slider.

**Returns:** The current slider position.

---

#### `void setPos(int pos)`

Sets the current position of the slider.

**Parameters:**
- `pos` - The new slider position

---

## Properties

### `pos` (Read/Write)

Sets or retrieves the current position of the slider.

**Type:** Integer (as string)

---

### `min` (Read/Write)

Sets or retrieves the starting position of the slider.

**Type:** Integer (as string)

**Default:** `"0"`

---

### `max` (Read/Write)

Sets or retrieves the maximum position of the slider.

**Type:** Integer (as string)

**Default:** `"100"`

---

## Events

### `onchange` (xgui::TextCallback)

Fired when the position of the slider changes.

**Parameters:**
- `text` - The current slider position

---

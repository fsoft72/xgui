# VBox

The `VBox` widget is a vertical box container that arranges its child widgets vertically from top to bottom. It derives from `Container` and provides control over child expansion and spacing.

## Class Hierarchy

```
Object
└── Widget
    └── Container
        └── VBox
```

## Methods

### Child Layout

#### `void setExpand(xgui::Widget * w, float expand)`

Sets the expansion mode for a VBox child specified by `w`. Children are always horizontally expanded.

**Parameters:**
- `w` - Pointer to the child Widget
- `expand` - Expansion mode:
  - `0.0` - Child is not vertically expanded
  - `1.0` - Child is expanded with a ratio based on its minimum vertical size

---

#### `float getExpand(xgui::Widget * w)`

Returns the expansion mode set by the `setExpand()` method for the specified VBox child.

**Parameters:**
- `w` - Pointer to the child Widget

**Returns:** The expansion mode value.

---

## Properties

### `space` (Read/Write)

Sets or retrieves the border space in pixels.

**Type:** Integer (as string)

**Default:** `"0"`

---

### `border` (Read/Write)

Controls whether the VBox is surrounded by a border.

**Values:**
- `"1"` - VBox is surrounded by a border
- `"0"` - No border is displayed

**Default:** `"0"`

---

## Events

This widget has no events beyond those inherited from `Widget`.

## Pushed Properties

Properties that are registered on all children of this container:

### `expand` (Read/Write)

Sets or retrieves the expansion mode for the child. See also `setExpand()` method.

**Type:** Float (as string)

**Values:**
- `"0.0"` - No vertical expansion
- `"1.0"` - Full vertical expansion

---

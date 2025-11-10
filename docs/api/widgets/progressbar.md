# Progressbar

The `Progressbar` widget displays a progress indicator showing the completion status of a task. It derives from `Widget` and provides methods for managing the progress counter position.

## Class Hierarchy

```
Object
└── Widget
    └── Progressbar
```

## Methods

### Progress Management

#### `int getPos()`

Returns the current position of the progress counter.

**Returns:** The current progress position.

---

#### `void setPos(int pos)`

Sets the current position of the progress counter.

**Parameters:**
- `pos` - The new progress position

---

## Properties

### `value` (Read/Write)

Sets or retrieves the current position of the progress counter.

**Type:** Integer (as string)

---

### `min` (Read/Write)

Sets or retrieves the starting position of the progress counter.

**Type:** Integer (as string)

**Default:** `"0"`

---

### `max` (Read/Write)

Sets or retrieves the maximum position of the progress counter.

**Type:** Integer (as string)

**Default:** `"100"`

---

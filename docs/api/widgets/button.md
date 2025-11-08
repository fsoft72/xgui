# Button

The `Button` widget is a clickable control that the user can interact with to provide input to the application. It derives from `Widget` and can display text, icons, or both. This class cannot be directly instantiated, but must be created using the `Master::CreateButton()` function.

## Class Hierarchy

```
Object
└── Widget
    └── Button
```

## Methods

### Icon Management

#### `int setIcon(Image * img)`

Sets the icon to be displayed on the button.

**Parameters:**
- `img` - Pointer to the Image to display on the button

**Returns:** Integer status code.

---

#### `xgui::Image * getIcon()`

Returns the image displayed on the button.

**Returns:** Pointer to the Image displayed on the button, or `NULL` if no image is set.

---

## Properties

### `image-path` (Read/Write)

Sets or retrieves the path of the file containing the image displayed on the button. If set to empty string, no image is displayed.

**Type:** String

**Default:** `""`

---

### `text` (Read/Write)

Sets or retrieves the text displayed on the button.

**Type:** String

**Default:** `""`

---

### `pressed` (Read/Write)

Controls the pressed state of toggle or radio buttons.

**Values:**
- `"1"` - Button is in pressed state
- `"0"` - Button is released

**Default:** `"0"`

---

### `alignment` (Read/Write)

Sets or retrieves the alignment of the button text.

**Values:**
- `"top-left"` - Align text to top-left corner
- `"top"` - Align text to top center
- `"top-right"` - Align text to top-right corner
- `"left"` - Align text to left center
- `"center"` - Center text
- `"right"` - Align text to right center
- `"bottom-left"` - Align text to bottom-left corner
- `"bottom"` - Align text to bottom center
- `"bottom-right"` - Align text to bottom-right corner

**Default:** `"center"`

---

## Events

### `onclick` (xgui::Callback)

Fired when the button is clicked.

---

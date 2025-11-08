# Entry

The `Entry` widget provides a single-line text input field. It derives from `Widget` and supports text editing, password mode, and various text manipulation methods.

## Class Hierarchy

```
Object
└── Widget
    └── Entry
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

## Properties

### `text` (Read/Write)

Sets or retrieves the text displayed in the entry.

**Type:** String

**Default:** `""`

---

### `editable` (Read/Write)

Controls whether the entry can be edited.

**Values:**
- `"1"` - Entry can be edited (text can be changed)
- `"0"` - Entry text is read-only

**Default:** `"1"`

---

### `maxlength` (Read/Write)

Sets or retrieves the maximum length of the entry text.

**Type:** Integer (as string)

**Default:** `0` (unlimited)

---

### `password-mode` (Read/Write)

Controls whether characters are displayed as password characters.

**Values:**
- `"1"` - Password characters (usually `*`) are displayed instead of typed characters
- `"0"` - Normal text display

**Default:** `"0"`

---

### `alignment` (Read/Write)

Sets or retrieves the text alignment of the entry box.

**Values:**
- `"left"` - Left-aligned text
- `"center"` - Center-aligned text
- `"right"` - Right-aligned text

**Default:** `"left"`

---

## Events

### `onsubmit` (xgui::TextCallback)

Fired when the Enter key is pressed while the entry widget has keyboard focus.

**Parameters:**
- `text` - The entry text

---

### `onchange` (xgui::TextCallback)

Fired when the user has taken an action that may have altered text in the entry widget.

**Parameters:**
- `text` - The altered entry text

**Note:** Returning `xgui::EVT_BLOCK` from the callback function prevents the text from changing.

---

# StyleManager

The `StyleManager` class manages visual styles for XGUI widgets. It derives from `Object` and provides methods for creating, storing, and retrieving style definitions used by styled items in views like lists and trees.

## Class Hierarchy

```
Object
└── StyleManager
```

## Methods

### Implementation

#### `xguimpl::StyleManager * getImpl()`

Returns a pointer to the implementation class.

**Returns:** Pointer to the platform-specific StyleManager implementation.

---

### Reference Counting

#### `int ref()`

Increments the reference counter and returns the incremented value.

**Returns:** The incremented reference count.

---

#### `int unref()`

Decrements the reference counter and returns the decremented value. If the counter reaches 0, the object is destroyed.

**Returns:** The decremented reference count.

---

### Style Access

#### `xgui::Style &operator[](std::string const &style_name)`

Returns a reference to the style named `style_name`.

**Parameters:**
- `style_name` - The name of the style to retrieve

**Returns:** Reference to the Style object.

---

#### `void setStyle(std::string const &style_name, Style const &st)`

Adds a new style to the StyleManager object.

**Parameters:**
- `style_name` - The name of the style
- `st` - A Style structure defining the style

---

#### `xgui::Style &getStyle(std::string const &style_name)`

Returns a reference to the style named `style_name`.

**Parameters:**
- `style_name` - The name of the style to retrieve

**Returns:** Reference to the Style object.

---

### Utility Methods

#### `unsigned int parseColorString(const std::string & color)`

Returns the integer representation of the specified color string.

**Parameters:**
- `color` - The color string to parse (e.g., `"#FF0000"`, `"red"`)

**Returns:** Integer representation of the color.

---

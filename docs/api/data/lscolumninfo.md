# LsColumnInfo

The `LsColumnInfo` class is an information structure used to describe a column for the List widget.

## Class Hierarchy

```
LsColumnInfo
```

## Description

This class provides a convenient way to specify column properties when working with List widgets. It encapsulates all the information needed to define a column's appearance and behavior.

## Constructors

### `LsColumnInfo()`

Initializes a column with a width of 100 pixels and default values for other properties.

---

### `LsColumnInfo(int cpos, const std::string & cid, const std::string & cname, int cwidth = 100, const std::string & calign = "left")`

Initializes a column with the specified information.

**Parameters:**
- `cpos` - The order position of the column in the List widget
- `cid` - The column ID
- `cname` - The column name displayed when column headers are visible
- `cwidth` - The column width in pixels (default: `100`)
- `calign` - The alignment of the text in the column: `"left"`, `"right"`, or `"center"` (default: `"left"`)

---

## Member Variables

### `int pos`

The order position of the column in the List widget.

**Type:** Integer

---

### `std::string name`

The column name displayed when column headers are visible.

**Type:** String

---

### `std::string id`

The column ID used to identify the column.

**Type:** String

---

### `int width`

The column width in pixels.

**Type:** Integer

**Default:** `100`

---

### `std::string align`

The alignment of the text in the column.

**Type:** String

**Values:**
- `"left"` - Left-aligned text
- `"center"` - Center-aligned text
- `"right"` - Right-aligned text

**Default:** `"left"`

---

# List

The `List` widget represents a group of data items displayed in a grid of rows and columns. It derives from `View` and gathers its data from the assigned model. The widget supports columns, icons, checkboxes, and sorting.

## Class Hierarchy

```
Object
└── Widget
    └── View
        └── List
```

## Methods

### Column Management

#### `void addColumn(int cpos, const std::string & cid, const std::string & cname, int cwidth = 100, const std::string & calign = "left")`

Adds a column to the list widget.

**Parameters:**
- `cpos` - Position where the column will be added
- `cid` - ID that identifies the column
- `cname` - Display name of the column
- `cwidth` - Minimum width of the column in pixels (default: `100`)
- `calign` - Alignment of the column: `"left"`, `"center"`, or `"right"` (default: `"left"`)

**Note:** On Win32 platform, if the column position `cpos` is set to 0, then the text is always left-aligned.

---

#### `void delColumn(const std::string & id)`

Removes the specified column from the list widget.

**Parameters:**
- `id` - ID of the column to remove

---

### Sorting

#### `void sort(const std::string & column_id, xgui::TextPairCallback * callback)`

Sorts the list items.

**Parameters:**
- `column_id` - ID of the column on which the items are to be sorted
- `callback` - Callback function for item comparison

---

### Check State Management

#### `virtual bool isChecked(xgui::Model * parent, int child_pos)`

Returns true if the item identified by `parent` and `child_pos` is in the "checked" state; false otherwise.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `child_pos` - Position of the child item

**Returns:** `true` if checked, `false` otherwise.

---

#### `virtual void check(xgui::Model * parent, int child_pos, bool state)`

Changes the check state of the item identified by `parent` and `child_pos`.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `child_pos` - Position of the child item
- `state` - If `true`, the state is changed to "checked"; otherwise to "unchecked"

---

#### `virtual void checkAll(xgui::Model * parent, bool state)`

Changes the check state of all list items.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `state` - If `true`, the state is changed to "checked"; otherwise to "unchecked"

---

## Properties

### `visible-headers` (Read/Write)

Controls the visibility of column headers.

**Values:**
- `"1"` - Column headers are visible and labeled with column titles
- `"0"` - Column headers are not displayed

**Default:** `"1"`

---

### `has-icons` (Read/Write)

Controls whether icons are displayed at the beginning of each row.

**Values:**
- `"1"` - An icon is displayed at the beginning of each row
- `"0"` - No icon is displayed

**Default:** `"0"`

---

### `has-checkboxes` (Read/Write)

Controls whether checkboxes are displayed at the beginning of each row.

**Values:**
- `"1"` - A checkbox is displayed at the beginning of each row
- `"0"` - No checkbox is displayed

**Default:** `"0"`

---

## Events

### `onselect` (xgui::TextStatusCallback)

Fired when the user changes the select state of an item in the list.

**Parameters:**
- `text` - The path (see `Widget::getPath()`) of the item (de)selected
- `status` - `1` if the item has been selected, `0` otherwise

---

### `onrefresh` (xgui::Callback)

Fired when the user inserts or removes an item or calls the `refresh()` method.

---

### `oncheck` (xgui::TextStatusCallback)

Fired when the user changes the check state of an item in the list.

**Parameters:**
- `text` - The path (see `Widget::getPath()`) of the item (un)checked
- `status` - `1` if the item has been checked, `0` otherwise

---

### `onheaderclick` (xgui::TextCallback)

Fired when the user clicks on a column header.

**Parameters:**
- `text` - The name of the clicked column

---

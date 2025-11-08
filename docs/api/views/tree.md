# Tree

The `Tree` widget displays a hierarchical list of items, such as headings in a document, entries in an index, or files and directories on a disk. It derives from `View` and gathers its data from the assigned model.

## Class Hierarchy

```
Object
└── Widget
    └── View
        └── Tree
```

## Methods

### Sorting

#### `void sort(xgui::Model * node, xgui::TextPairCallback * callback)`

Sorts the children of the specified tree node.

**Parameters:**
- `node` - Pointer to the Model node whose children will be sorted
- `callback` - Callback function for item comparison

---

### Node Expansion

#### `void expand(xgui::Model * node)`

Expands the list of child items associated with the specified node item.

**Parameters:**
- `node` - Pointer to the Model node to expand

---

#### `void collapse(xgui::Model * node)`

Collapses the list of child items associated with the specified node item.

**Parameters:**
- `node` - Pointer to the Model node to collapse

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

Changes the check state of all child items of the specified node.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `state` - If `true`, the state is changed to "checked"; otherwise to "unchecked"

---

## Properties

### `has-icons` (Read/Write)

Controls whether icons are displayed near the text of each item.

**Values:**
- `"1"` - An icon is displayed near the text of each item
- `"0"` - No icon is displayed

**Default:** `"0"`

---

### `has-checkboxes` (Read/Write)

Controls whether checkboxes are displayed near the text of each item.

**Values:**
- `"1"` - A checkbox is displayed near the text of each item
- `"0"` - No checkbox is displayed

**Default:** `"0"`

---

## Events

### `onselect` (xgui::TextStatusCallback)

Fired when the user changes the select state of a tree node.

**Parameters:**
- `text` - The path (see `Widget::getPath()`) of the item (de)selected
- `status` - `1` if the item has been selected, `0` otherwise

---

### `onrefresh` (xgui::Callback)

Fired when the user inserts or removes an item or calls the `refresh()` method.

---

### `oncheck` (xgui::TextStatusCallback)

Fired when the user changes the check state of a tree node.

**Parameters:**
- `text` - The path (see `Widget::getPath()`) of the item (un)checked
- `status` - `1` if the item has been checked, `0` otherwise

---

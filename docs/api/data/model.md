# Model

The `Model` class is used as a repository for data shown in views. It derives from `TreeNode` and provides data management functionality for view widgets. Views are widgets derived from the `View` class, and a Model is assigned to them. The same Model can be assigned to different views so the same data can be displayed in different widgets.

Each Model object is a node of a tree and can contain one or more children. The data associated with a model node is represented by a list of strings.

This class cannot be directly instantiated but must be created using the `Master::CreateModel()` function.

## Class Hierarchy

```
Object
└── TreeNode
    └── Model
```

## Methods

### Child Management

#### `void appendChild(std::vector<std::string> const & string_list, std::string const &style_name = "default")`

Adds a new child to the end of the child list of this model node. The child is filled with the specified vector of strings.

**Parameters:**
- `string_list` - Vector of strings to fill the child with
- `style_name` - Name of a style to be used by views when displaying this item (default: `"default"`)

**Note:** The style is defined in the StyleManager object. This method cannot be dynamically called.

---

#### `void appendChild(std::string const & val, std::string const &style_name = "default")`

Adds a new child to the end of the child list of this model node. The child is filled with just one string.

**Parameters:**
- `val` - String value for the child
- `style_name` - Name of a style to be used by views when displaying this item (default: `"default"`)

**Note:** The style is defined in the StyleManager object. This method cannot be dynamically called.

---

#### `void appendChild(TreeNode * child)`

Adds the specified child to the end of the child list of this model node.

**Parameters:**
- `child` - Pointer to the TreeNode to add

---

#### `void insertChild(TreeNode * child, int pos)`

Inserts the specified child at the specified position of the child list of this model node.

**Parameters:**
- `child` - Pointer to the TreeNode to insert
- `pos` - Position in the child list

---

#### `void removeChild(int pos)`

Removes the child at the specified position of the child list.

**Parameters:**
- `pos` - Position of the child to remove

---

#### `void removeAllChildren()`

Removes all the children of the model.

---

#### `void setChild(TreeNode * new_child, int pos)`

Substitutes the child at the specified position of the child list with `new_child`.

**Parameters:**
- `new_child` - Pointer to the new TreeNode
- `pos` - Position in the child list

---

### Child Access

#### `xgui::Model * getChild(int pos)`

Returns the child at the specified position of the child list.

**Parameters:**
- `pos` - Position in the child list

**Returns:** Pointer to the child Model.

---

#### `xgui::Model * findByPath(std::string const &path)`

Returns a child given its path (see `TreeNode::path()`).

**Parameters:**
- `path` - The path of the node to find

**Returns:** Pointer to the Model, or `NULL` if not found.

---

### Flags

#### `void setFlag(unsigned long flag)`

Sets a flag for this model node.

**Parameters:**
- `flag` - Bit mask specifying the bits that are to be set

---

#### `void unsetFlag(unsigned long flag)`

Unsets a flag for this model node.

**Parameters:**
- `flag` - Bit mask specifying the bits that are to be unset

---

#### `bool getFlagStatus(unsigned long flag)`

Returns true if the specified flags are set.

**Parameters:**
- `flag` - Bit mask specifying the bits that are to be checked

**Returns:** `true` if the flags are set, `false` otherwise.

---

### Style Management

#### `void setStyle(std::string const & style_name)`

Sets the visual style to be used by style-managed views when displaying this model element.

**Parameters:**
- `style_name` - Name of the style

---

#### `std::string const & getStyle()`

Returns the name of the style assigned to this model element.

**Returns:** The style name.

---

### String List Management

#### `void addString(std::string const &str)`

Adds a string to the string list of this model element.

**Parameters:**
- `str` - The string to add

---

#### `void delString(int pos)`

Removes the string at the specified position of the string list of this model element.

**Parameters:**
- `pos` - Position of the string to remove

---

#### `void setString(std::string const &str, int pos)`

Substitutes the string at the specified position of the string list of this model element with `str`.

**Parameters:**
- `str` - The new string
- `pos` - Position in the string list

---

#### `const std::string & getString(int pos)`

Returns the string at the specified position of the string list of this model element.

**Parameters:**
- `pos` - Position in the string list

**Returns:** The string at the specified position.

---

#### `void clearStrings()`

Removes all strings of this model element.

---

### View Update

#### `void updateViews()`

Refreshes all the views associated with this model (calling the `refresh()` method on each view).

---

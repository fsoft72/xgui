# TreeNode

The `TreeNode` class is used to implement a tree structure and represents a single node that can have one or more children nodes. It derives from `Object` and is used as a base class of the `xgui::Model` class.

## Class Hierarchy

```
Object
└── TreeNode
```

## Methods

### Child Management

#### `void appendChild(TreeNode * child)`

Adds the specified tree node as a child of this node. The child is added to the tail of the child list.

**Parameters:**
- `child` - Pointer to the TreeNode to add

---

#### `void insertChild(TreeNode * child, int pos)`

Inserts the specified tree node as a child of this node. The child is added at the specified position of the child list.

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

Removes all the children of the tree node.

---

#### `unsigned int size()`

Returns the number of children of this tree node.

**Returns:** The number of children.

---

### Child Access

#### `TreeNode * getChild(int pos)`

Returns a reference to the child at the specified position in the child list.

**Parameters:**
- `pos` - Position in the child list

**Returns:** Pointer to the child TreeNode.

---

#### `void setChild(TreeNode * new_child, int pos)`

Substitutes the child at the specified position in the child list with `new_child`.

**Parameters:**
- `new_child` - Pointer to the new TreeNode
- `pos` - Position in the child list

---

#### `xgui::TreeNode * findByPath(std::string const &path)`

Returns a child given its path.

**Parameters:**
- `path` - The path of the node to find

**Returns:** Pointer to the TreeNode, or `NULL` if not found.

---

### Tree Navigation

#### `std::string path()`

Returns a string that describes the ID of this node and of all its parent nodes.

**Format:** `"<root_node_id>[/<parent_node_id>[...]]/<node_id>"`

**Returns:** The path string.

---

#### `TreeNode * getParent()`

Returns the parent node of the tree node.

**Returns:** Pointer to the parent TreeNode, or `NULL` if this is the root.

---

### Iteration

#### `iterator begin()`

Returns an iterator object referring to the first child of this node.

**Returns:** Iterator to the first child.

---

#### `iterator end()`

Returns an iterator object referring to the last child of this node.

**Returns:** Iterator to the last child.

---

### Reference Counting

#### `void ref()`

Increments the reference counter of this node.

---

#### `void unref()`

Decrements the reference counter of this node, and destroys the node if it reaches 0.

---

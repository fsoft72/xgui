# Container

The `Container` class is the base class for all widgets that can contain child widgets. It derives from `Widget` and provides methods for managing and accessing child widgets.

## Class Hierarchy

```
Object
└── Widget
    └── Container
```

## Methods

### Child Management

#### `xgui::Widget * findChild(std::string const &child_name)`

Returns the child identified by `child_name`, or NULL if no child is found. The child is searched in the container and recursively in the children of all the contained containers.

**Parameters:**
- `child_name` - The ID of the child widget to find

**Returns:** Pointer to the child Widget, or `NULL` if not found.

---

#### `xgui::Widget * findDirectChild(std::string const &child_name)`

Returns the child identified by `child_name`, or NULL if no child is found. No recursive search is performed; just the direct children of the container are searched.

**Parameters:**
- `child_name` - The ID of the child widget to find

**Returns:** Pointer to the child Widget, or `NULL` if not found.

---

#### `xgui::Widget * getChild(int pos)`

Returns the child widget at the specified position in the contained children list, or NULL if there is no child at the specified position.

**Parameters:**
- `pos` - The position index of the child (0-based)

**Returns:** Pointer to the child Widget at the specified position, or `NULL` if invalid position.

---

## Properties

This widget has no additional properties beyond those inherited from `Widget`.

## Events

This widget has no events beyond those inherited from `Widget`.

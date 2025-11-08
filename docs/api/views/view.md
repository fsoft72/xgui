# View

The `View` class is the base class for widgets that display data from a model. It derives from `Widget` and provides functionality for model management and item selection.

## Class Hierarchy

```
Object
└── Widget
    └── View
```

## Methods

### Model Management

#### `void setModel(xgui::Model * model)`

Sets a new model for the view. The model holds all the data that the view will display.

**Parameters:**
- `model` - Pointer to the Model to assign to this view

---

#### `xgui::Model * getModel()`

Returns the model assigned to the view.

**Returns:** Pointer to the assigned Model, or `NULL` if no model is assigned.

---

#### `void refresh()`

Updates the view contents by retrieving the data from the assigned model.

---

### Selection Management

#### `bool isSelected(xgui::Model * parent, int child_pos)`

Returns true if the model item identified by `parent` and `child_pos` is currently selected in this view.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `child_pos` - Position of the child item

**Returns:** `true` if selected, `false` otherwise.

---

#### `void select(xgui::Model * parent, int child_pos, bool state)`

Changes the selected state of the model item identified by `parent` and `child_pos` in this view.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `child_pos` - Position of the child item
- `state` - If `true`, the state is changed to "selected"; otherwise to "unselected"

---

#### `void selectAll(xgui::Model * parent, bool state)`

Changes the selected state of all the children of the model item identified by `parent` in this view.

**Parameters:**
- `parent` - Pointer to the parent Model item
- `state` - If `true`, the state is changed to "selected"; otherwise to "unselected"

---

## Properties

This widget has no additional properties beyond those inherited from `Widget`.

## Events

This widget has no events beyond those inherited from `Widget`.

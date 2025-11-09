# Widget Reference Guide

This document provides a quick reference for all widgets demonstrated in the complex-example application.

## Widget Catalog

### Window
- **Purpose**: Top-level application window
- **Creation**: Inherits from `xgui::Window`
- **Properties**: `text` (title), `size`, `position`
- **Events**: `onclose`

### Menu
- **Purpose**: Application menu bar and dropdown menus
- **Creation**: `Master::CreateMenu(Window*, bool visible)`
- **Methods**: `addItem(parent_id, item_id, label)`
- **Events**: `onselect` (TextStatusCallback)
- **Example**: File, Edit, Help menus

### Tab
- **Purpose**: Tabbed interface container
- **Creation**: `Master::CreateTab(Container*, alignment)`
- **Methods**: `addPage(widget, label)`
- **Events**: `onselect`
- **Properties**: Alignment can be "top", "bottom", "left", "right"

### Label
- **Purpose**: Static text display
- **Creation**: `Master::CreateLabel(Container*, text)`
- **Properties**: `text`, `font-weight`, `font-size`, `font-style`
- **Use Cases**: Info display, headings, status messages

### Button
- **Purpose**: Clickable action trigger
- **Creation**: `Master::CreateButton(Container*, text)`
- **Properties**: `text`, `toggle` (for toggle buttons), `value`
- **Events**: `onclick`
- **Features**: Can be regular or toggle mode

### Checkbox
- **Purpose**: Boolean on/off toggle
- **Creation**: `Master::CreateCheckbox(Container*, label)`
- **Properties**: `value` ("0" or "1")
- **Events**: `onclick`
- **Example**: Enable/disable advanced features

### Entry
- **Purpose**: Single-line text input
- **Creation**: `Master::CreateEntry(Container*, initial_text)`
- **Properties**: `text`, `password`, `editable`, `max-length`
- **Events**: `onchange` (TextCallback), `onsubmit`
- **Modes**: Normal text or password (masked)

### Slider
- **Purpose**: Range value selection
- **Creation**: `Master::CreateSlider(Container*, min, max)`
- **Properties**: `value`, `min`, `max`
- **Events**: `onchange`
- **Example**: 0-100 range, linked to progress bar

### ProgressBar
- **Purpose**: Visual progress/value indicator
- **Creation**: `Master::CreateProgressbar(Container*, min, max)`
- **Properties**: `value`, `min`, `max`
- **Use Cases**: Linked to slider, file download progress

### Spin
- **Purpose**: Numeric input with increment/decrement buttons
- **Creation**: `Master::CreateSpin(Container*, min, max)`
- **Properties**: `value`, `min`, `max`
- **Events**: `onchange`, `onsubmit`
- **Example**: Counter from 0 to 100

### Combobox
- **Purpose**: Dropdown selection list
- **Creation**: `Master::CreateCombobox(Container*, editable)`
- **Methods**: `addItem(text)`, `removeItem(index)`
- **Properties**: `text` (selected item)
- **Events**: `onselect` (TextStatusCallback)
- **Modes**: Editable or read-only

### List
- **Purpose**: Multi-column data grid
- **Creation**: `Master::CreateList(Container*, StyleManager*)`
- **Methods**:
  - `addColumn(index, id, label, width)`
  - `setModel(Model*)`
  - `check(model, row, checked)` for checkbox mode
- **Properties**: `has-checkboxes`, `expand`
- **Events**: `onselect`, `oncheck`, `onheaderclick`
- **Features**: Multiple columns, selection, checkboxes

### Tree
- **Purpose**: Hierarchical tree view
- **Creation**: `Master::CreateTree(Container*, StyleManager*)`
- **Methods**: `setModel(Model*)`
- **Events**: `onselect`, `onexpand`, `oncheck`
- **Features**: Expandable nodes, parent-child relationships

### Model
- **Purpose**: Data container for List and Tree
- **Creation**: `Master::CreateModel()`
- **Methods**:
  - `appendChild(Model*)` or `appendChild(vector<string>)`
  - `addString(text)`
  - `getString(index)`
  - `findByPath(path)`
- **Structure**: Hierarchical tree of data rows

### Calendar
- **Purpose**: Date selection widget
- **Creation**: `Master::CreateCalendar(Container*)`
- **Events**: `onselect` (TextStatusCallback with date string)
- **Format**: Date returned in YYYY-MM-DD format

### ImageView
- **Purpose**: Display images with scrolling
- **Creation**: `Master::CreateImageView(Container*, Image*)`
- **Properties**: Alignment, scrolling
- **Use Cases**: Photo viewer, icon display

### Toolbar
- **Purpose**: Application toolbar with buttons
- **Creation**: `Master::CreateToolbar(Container*)`
- **Methods**:
  - `addButton(id, label, icon)`
  - `addSeparator()`
- **Events**: `onclick` (TextStatusCallback with button id)

### Frame
- **Purpose**: Labeled container for grouping widgets
- **Creation**: `Master::CreateFrame(Container*, label)`
- **Properties**: `enabled` (enable/disable all children)
- **Use Cases**: Settings groups, feature sections

### VBox
- **Purpose**: Vertical layout container
- **Creation**: `Master::CreateVBox(Container*, spacing, border)`
- **Properties**: `expand` (child expansion ratio)
- **Layout**: Stacks children vertically

### HBox
- **Purpose**: Horizontal layout container
- **Creation**: `Master::CreateHBox(Container*, spacing, border)`
- **Properties**: `expand` (child expansion ratio)
- **Layout**: Arranges children horizontally

### Space
- **Purpose**: Flexible spacer for layout alignment
- **Creation**: `Master::CreateSpace(Container*, min_width, min_height)`
- **Properties**: `expand` (usually "1.0" to fill space)
- **Use Cases**: Push widgets to edges, vertical spacing

## Callback Types

### Callback
- **Signature**: `int callback(Widget* w)`
- **Use Cases**: onclick, onchange (for valueless events)
- **Implementation**: `CppMCallback<Class>` or `CppFCallback`

### TextCallback
- **Signature**: `int callback(Widget* w, string const &text)`
- **Use Cases**: Entry onchange, text-based events
- **Implementation**: `CppMTextCallback<Class>` or `CppFTextCallback`

### TextStatusCallback
- **Signature**: `int callback(Widget* w, string const &id, int status)`
- **Use Cases**: List/Tree selection, Menu selection, Calendar selection
- **Implementation**: `CppMTextStatusCallback<Class>` or `CppFTextStatusCallback`
- **Parameters**:
  - `id`: Item identifier or path
  - `status`: Selection state (1=selected, 0=deselected)

## Property System

All widgets support dynamic properties via:
```cpp
widget->set("property", "value");
widget->get("property", value_out);
```

### Common Properties
- `text`: Display text or value
- `value`: Numeric or boolean value
- `enabled`: Enable/disable widget ("0" or "1")
- `visible`: Show/hide widget ("0" or "1")
- `expand`: Layout expansion ratio ("0.0" to "1.0")
- `size`: Widget size "width height" (e.g., "800 600")
- `position`: Window position "x y" or "center"

### Widget-Specific Properties
- **Entry**: `password`, `editable`, `max-length`
- **Button**: `toggle` (toggle mode)
- **List**: `has-checkboxes`
- **Label**: `font-weight`, `font-size`, `font-style`
- **Slider/ProgressBar/Spin**: `min`, `max`, `value`

## Event Return Values

All event handlers must return:
- `EVT_PROPAGATE`: Allow event to propagate to other handlers
- `EVT_BLOCK`: Stop event propagation

## Linked Widget Behaviors in Example

1. **Slider → ProgressBar**
   - Moving slider updates progress bar value in real-time
   - Demonstrates widget synchronization

2. **Checkbox → Frame**
   - Checking enables settings frame
   - Unchecking disables settings frame
   - Shows enable/disable cascading

3. **Entry → Label**
   - Typing name shows greeting
   - Real-time text transformation

4. **Spinner → Label**
   - Changing spinner updates value label
   - Numeric value display

5. **Calendar → Label**
   - Selecting date updates date label
   - Date formatting example

6. **List/Tree → Status**
   - Selection updates status bar
   - Cross-widget communication

7. **Toolbar → Status**
   - Clicking toolbar button updates status
   - Action feedback

## Best Practices

1. **Memory Management**
   - Delete callbacks in destructor
   - Use `Master::DestroyWidget()` for widgets
   - Use `Master::DestroyObject()` for objects

2. **Event Handling**
   - Always return `EVT_PROPAGATE` unless blocking needed
   - Check widget state in handlers
   - Log important events for debugging

3. **Layout**
   - Use VBox/HBox for structure
   - Set `expand` on widgets that should grow
   - Use Space for alignment

4. **Properties**
   - Set properties after creation
   - Use `get()` to read current state
   - Properties are strings, convert as needed

5. **Models**
   - Create models separately
   - Populate before setting on widget
   - Use `appendChild()` for tree structure

## See Also

- `main.cpp` - Full implementation example
- `README.md` - Overview and building instructions
- `/xgui/*.h` - Widget header files with detailed API

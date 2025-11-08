# API Reference

This section contains the complete API reference for all XGUI classes and widgets.

## Class Hierarchy

XGUI follows a clear object-oriented hierarchy with `Object` as the base class for all XGUI components.

```
Object
├── Widget
│   ├── Button
│   ├── Calendar
│   ├── Checkbox
│   ├── Container
│   │   ├── Frame
│   │   ├── HBox
│   │   ├── VBox
│   │   ├── Window
│   │   └── Tab
│   ├── Entry
│   ├── ImageView
│   ├── Label
│   ├── Menu
│   ├── Progressbar
│   ├── Slider
│   ├── Space
│   ├── Spin
│   ├── Toolbar
│   └── View
│       ├── Combobox
│       ├── List
│       └── Tree
├── Image
├── Master
├── TreeNode
│   └── Model
└── StyleManager
```

## Core Classes

These are the fundamental classes that form the foundation of XGUI:

- **[Object](core/object.md)** - Base class for all XGUI objects with property and method management
- **[Widget](core/widget.md)** - Base class for all UI components with event handling
- **[Master](core/master.md)** - Main application controller and widget factory
- **[Image](core/image.md)** - Image loading and management
- **[StyleManager](core/stylemanager.md)** - Visual style management for widgets

## Widgets

Standard UI components that derive from the Widget class:

### Input Widgets
- **[Button](widgets/button.md)** - Clickable button widget
- **[Checkbox](widgets/checkbox.md)** - Toggle checkbox control
- **[Entry](widgets/entry.md)** - Single-line text input
- **[Slider](widgets/slider.md)** - Value selection slider
- **[Spin](widgets/spin.md)** - Numeric spinbox control

### Display Widgets
- **[Label](widgets/label.md)** - Static text display
- **[ImageView](widgets/imageview.md)** - Image display widget
- **[Progressbar](widgets/progressbar.md)** - Progress indicator
- **[Calendar](widgets/calendar.md)** - Date selection widget

### Navigation Widgets
- **[Menu](widgets/menu.md)** - Menu system with items and submenus
- **[Toolbar](widgets/toolbar.md)** - Toolbar with buttons

### Layout Widgets
- **[Space](widgets/space.md)** - Flexible spacing for layout management

## Containers

Container widgets can hold and organize other widgets:

- **[Container](containers/container.md)** - Base class for all containers
- **[Window](containers/window.md)** - Top-level window
- **[Frame](containers/frame.md)** - Decorative border container
- **[HBox](containers/hbox.md)** - Horizontal box layout
- **[VBox](containers/vbox.md)** - Vertical box layout
- **[Tab](containers/tab.md)** - Tabbed container with multiple pages

## Views

Model-driven widgets for displaying structured data:

- **[View](views/view.md)** - Base class for model-driven views
- **[List](views/list.md)** - Multi-column list widget
- **[Tree](views/tree.md)** - Hierarchical tree widget
- **[Combobox](views/combobox.md)** - Dropdown list with optional entry

## Data Structures

Classes for managing structured data:

- **[Model](data/model.md)** - Data model for views
- **[TreeNode](data/treenode.md)** - Tree node implementation
- **[LsColumnInfo](data/lscolumninfo.md)** - List column information
- **[TbButtonInfo](data/tbbuttoninfo.md)** - Toolbar button information
- **[FileExtension](data/fileextension.md)** - File type descriptor for dialogs

## Callbacks

Event handling through the callback system:

- **[Callback Overview](callbacks/index.md)** - Understanding the callback system
- **[Callback](callbacks/callback.md)** - Basic callback interface
- **[TextCallback](callbacks/textcallback.md)** - Callback with text parameter
- **[TextStatusCallback](callbacks/textstatuscallback.md)** - Callback with text and status
- **[TextPairCallback](callbacks/textpaircallback.md)** - Callback with two text parameters

## Quick Reference

### Creating Widgets

All widgets must be created using factory methods from the `Master` class:

```cpp
xgui::Master *master = xgui::Master::Instance();

// Create various widgets
xgui::Window *window = master->CreateWindow();
xgui::Button *button = master->CreateButton();
xgui::Entry *entry = master->CreateEntry();
xgui::List *list = master->CreateList();
```

### Property System

All objects support a dynamic property system:

```cpp
// Set properties
widget->set("property-name", "value");

// Get properties
std::string value;
widget->get("property-name", value);

// Check properties
xgui::PropertyType type = widget->hasProperty("property-name");
```

### Event Handling

Link callbacks to widget events:

```cpp
class MyCallback : public xgui::Callback {
public:
    virtual void call() {
        // Handle event
    }
};

MyCallback *callback = new MyCallback();
button->linkEvent("onclick", callback);
```

### Container Management

Add widgets to containers:

```cpp
xgui::Window *window = master->CreateWindow();
xgui::VBox *vbox = master->CreateVBox();
xgui::Button *button = master->CreateButton();

vbox->add(button);
window->add(vbox);
```

## Common Patterns

### Modal Dialogs

```cpp
// Create modal window
xgui::Window *parent = master->CreateWindow();
xgui::Window *modal = master->CreateWindow(parent);
```

### File Dialogs

```cpp
xgui::FileExtensionsVector filters;
filters.push_back(xgui::FileExtension("Text Files", "*.txt"));

xgui::StrVector files = xgui::Master::OpenFileDialog(
    window,
    filters,
    "Open File",
    "",
    false  // multiselection
);
```

### Message Dialogs

```cpp
int result = xgui::Master::MessageDialog(
    window,
    "Confirmation",
    "Are you sure?",
    "question",
    "yes-no"
);
```

## See Also

- [Getting Started](../getting-started.md) - Tutorial and basic examples
- [Home](../index.md) - Main documentation page

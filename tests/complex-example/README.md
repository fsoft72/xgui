# XGUI Complex Widget Example

A comprehensive C++ example showcasing all available widgets in the XGUI library with interactive events and real-time GUI updates.

## Overview

This example demonstrates how to use all XGUI widgets in a single application, including:

### Basic Controls (Tab 1)
- **Label** - Static text display with formatting
- **Button** - Action buttons with click counting
- **Toggle Button** - Stateful button that maintains on/off state
- **Checkbox** - Boolean toggle that enables/disables advanced features

### Input Widgets (Tab 2)
- **Entry** - Text input field (normal and password mode)
- **Slider** - Range selection widget (0-100) that updates a progress bar in real-time
- **ProgressBar** - Visual progress indicator linked to the slider
- **Spin** - Numeric spinner with increment/decrement buttons
- **Combobox** - Dropdown selection list

### Data Views (Tab 3)
- **List** - Multi-column data grid with selection
- **Tree** - Hierarchical tree view with expandable nodes
- **Calendar** - Date picker widget
- **ImageView** - (Placeholder for image display)

### Advanced Widgets (Tab 4)
- **Toolbar** - Application toolbar with multiple action buttons
- **Frame** - Grouped container with label
- **Tab** - Tabbed interface container
- **Menu** - Menu bar with File, Edit, and Help menus

### Layout Containers
- **VBox** - Vertical box layout
- **HBox** - Horizontal box layout
- **Space** - Flexible spacer for layout alignment

## Features

### Interactive Events
All widgets demonstrate event handling with:
- **Console logging** - Every user interaction is logged to the console with event details
- **GUI updates** - Changes in one widget affect other widgets in real-time

### Widget Interactions
- **Slider → Progress Bar** - Moving the slider automatically updates the progress bar
- **Checkbox → Frame** - Checkbox enables/disables the settings frame
- **Entry → Label** - Text entry updates greeting label in real-time
- **Spinner → Label** - Spinner value is displayed in a label
- **Calendar → Label** - Selected date is displayed in a label
- **List/Tree → Status** - Selection updates the status label

### Console Output
Every interaction produces console output showing:
- Event type (onclick, onchange, onselect, etc.)
- Widget affected
- New values or states
- Related data

Example console output:
```
[EVENT] Action button clicked! Total clicks: 5
[EVENT] Slider changed: 75
[EVENT] Name entry changed: 'John Doe'
[EVENT] Calendar date selected: 2025-11-09
[EVENT] List item selected: Item 2 = 200
[EVENT] Menu item selected: about_item
```

## Building

### Prerequisites
- GTK+ 3.0 development libraries
- g++ compiler
- make

### Compile

```bash
make -f Makefile.gtk complex-example
```

This will:
1. Compile all XGUI library objects
2. Compile the complex-example source
3. Link everything into `output/complex-example`

## Running

```bash
./output/complex-example
```

The application will:
1. Display a startup message in the console
2. Open a 900x700 window with 4 tabs
3. Log all user interactions to the console
4. Allow you to explore all widgets and their events

## Code Structure

The example is implemented as a single class `ComplexWidgetDemo` that:

1. **Inherits from Window** - Main application window
2. **Creates all widgets** - Organized in separate creation methods per tab
3. **Defines callbacks** - Member functions for each event type
4. **Links events** - Connects widgets to their event handlers
5. **Manages lifecycle** - Proper initialization and cleanup

### Key Methods

- `createMenu()` - Sets up the menu bar
- `createBasicTab()` - Creates basic control widgets
- `createInputTab()` - Creates input widgets with linked behaviors
- `createDataTab()` - Creates data view widgets
- `createAdvancedTab()` - Creates toolbar and advanced controls
- `onActionButton()`, `onSliderChange()`, etc. - Event handlers

### Event Handler Pattern

```cpp
int onSliderChange(Widget * w)
{
    std::string value;
    slider_widget->get("value", value);

    std::cout << "[EVENT] Slider changed: " << value << std::endl;

    // Update linked progress bar
    progress_bar->set("value", value);

    return EVT_PROPAGATE;
}
```

## Learning Points

This example teaches:

1. **Widget Creation** - How to create all widget types using Master factory methods
2. **Event Handling** - Different callback types (Callback, TextCallback, TextStatusCallback)
3. **Layout Management** - Using VBox, HBox, Frame, and Tab containers
4. **Property System** - Getting and setting widget properties
5. **Model-View Pattern** - Using Model with List and Tree widgets
6. **Widget Linking** - Creating interactive behaviors between widgets
7. **Memory Management** - Proper callback allocation and deletion

## Customization

You can extend this example by:
- Adding custom widgets to any tab
- Creating new event interactions
- Implementing additional widget linking behaviors
- Adding image loading for ImageView
- Extending the menu with more actions
- Creating dialog windows

## See Also

- `/examples/*.json` - JSON-based UI definitions
- `/tests/plan/main.cpp` - Advanced XGUI object inspector
- `/tests/xgui-example/main.cpp` - JSON loader example
- `/xgui/*.h` - Widget header files with API documentation

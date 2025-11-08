# XGUI Examples

This directory contains a comprehensive collection of JSON examples demonstrating all XGUI capabilities, from simple interfaces to complex applications.

## Important Note

**Standalone vs. Programmatic Examples**: Examples 01-10 are fully standalone and work with the `xgui-example` viewer. Examples 11-18 reference data models (for List, Tree, and Combobox widgets) which need to be populated programmatically in C++ code. These examples demonstrate the correct JSON structure but won't display data when loaded with the simple viewer alone. See the [C++ Examples](#using-models-in-c) section for how to populate models.

## Simple Examples (2-3 widgets)

These examples demonstrate basic XGUI usage with minimal widgets:

### 01-simple-button.json
- **Widgets**: Window, VBox, Label, Button
- **Features**: Basic button and label layout
- **Complexity**: Beginner
- **Description**: The simplest possible XGUI application with a label and button

### 02-simple-form.json
- **Widgets**: Window, VBox, HBox, Label, Entry, Button
- **Features**: Basic form with text input
- **Complexity**: Beginner
- **Description**: Simple name entry form demonstrating horizontal and vertical layouts

### 03-checkbox-demo.json
- **Widgets**: Window, VBox, Label, Checkbox
- **Features**: Multiple checkboxes with different states
- **Complexity**: Beginner
- **Description**: Settings panel with checkboxes for preferences

### 04-slider-control.json
- **Widgets**: Window, VBox, Label, Slider
- **Features**: Slider with min/max values
- **Complexity**: Beginner
- **Description**: Volume control slider demonstration

### 05-progress-bar.json
- **Widgets**: Window, VBox, Label, ProgressBar
- **Features**: Progress indicator
- **Complexity**: Beginner
- **Description**: Download progress indicator example

## Medium Complexity Examples

These examples combine multiple widgets and demonstrate layout techniques:

### 06-registration-form.json
- **Widgets**: Window, VBox, HBox, Frame, Label, Entry, Checkbox, Button, Space
- **Features**: Form layout, password field, grouped controls, button alignment
- **Complexity**: Intermediate
- **Description**: User registration form with validation elements and proper layout

### 07-tabbed-settings.json
- **Widgets**: Window, VBox, Tab, TabPage, Checkbox, Button, Spin, Label, HBox
- **Features**: Tabbed interface, multiple setting categories
- **Complexity**: Intermediate
- **Description**: Application settings dialog with multiple tabs for different categories

### 08-calendar-picker.json
- **Widgets**: Window, VBox, HBox, Label, Calendar, Entry, Button
- **Features**: Date selection, read-only entry display
- **Complexity**: Intermediate
- **Description**: Date picker interface with calendar widget

### 09-image-viewer.json
- **Widgets**: Window, VBox, HBox, Button, ImageView, Label
- **Features**: Image display, scrolling, toolbar controls
- **Complexity**: Intermediate
- **Description**: Image viewer with zoom and navigation controls

### 10-spin-and-slider.json
- **Widgets**: Window, VBox, HBox, Frame, Label, Slider, Spin, ProgressBar
- **Features**: Numeric input controls, value sliders, progress indication
- **Complexity**: Intermediate
- **Description**: Temperature and quantity controls demonstrating sliders and spinners

## Complex Examples

These examples showcase advanced features and complete applications:

### 11-data-list.json
- **Widgets**: Window, VBox, HBox, Button, Entry, List, ListColumn, Label, Space
- **Features**: Multi-column data grid, checkboxes, search functionality, data models
- **Complexity**: Advanced
- **Description**: Contact list manager with full CRUD operations and search

### 12-tree-view.json
- **Widgets**: Window, VBox, HBox, Button, Tree, Frame, Label
- **Features**: Hierarchical tree data, icons, split-panel layout, detail view
- **Complexity**: Advanced
- **Description**: File system browser with tree navigation and property panel

### 13-menu-toolbar-app.json
- **Widgets**: Window, VBox, Menu, MenuItem, Toolbar, ToolbarButton, ToolbarCheckButton, ToolbarSeparator, Entry, Label
- **Features**: Complete menu system, toolbar, separators, check menu items
- **Complexity**: Advanced
- **Description**: Text editor application with full menu and toolbar integration

### 14-dashboard.json
- **Widgets**: Window, VBox, HBox, Frame, Label, ProgressBar, List, ListColumn, Button
- **Features**: Complex multi-panel layout, real-time monitoring, mixed widget types
- **Complexity**: Advanced
- **Description**: System monitoring dashboard with CPU/memory/disk meters and process list

### 15-multi-tab-application.json
- **Widgets**: All major widget types combined
- **Features**: Menu system, multi-tab interface, lists, calendar, comboboxes, frames
- **Complexity**: Expert
- **Description**: Complete project management suite demonstrating enterprise-level UI

## Specialized Examples

### 16-layout-showcase.json
- **Widgets**: Window, VBox, HBox, Frame, Button, Label, Entry, Checkbox, Space
- **Features**: Expand ratios, spacing, alignment, frame grouping
- **Complexity**: Intermediate
- **Description**: Demonstrates various layout techniques and the expand property

### 17-radio-buttons.json
- **Widgets**: Window, VBox, Frame, Button (used as radio buttons)
- **Features**: Radio button groups (using pressed state), mutually exclusive selection
- **Complexity**: Intermediate
- **Description**: Payment and shipping selection with radio button groups

### 18-all-widgets.json
- **Widgets**: Every available XGUI widget
- **Features**: Comprehensive widget showcase across multiple tabs
- **Complexity**: Advanced
- **Description**: Complete reference showing all widgets with various states and configurations

## Widget Categories Demonstrated

### Basic Controls
- **Button**: Normal, toggle, radio, disabled states
- **Label**: Various alignments and text styles
- **Entry**: Regular, password, read-only, max length
- **Checkbox**: Checked, unchecked, disabled states

### Value Controls
- **Slider**: Range selection with min/max values
- **Spin**: Numeric input with increment/decrement
- **ProgressBar**: Progress indication with position control
- **Combobox**: Dropdown selection, editable and non-editable

### Layout Containers
- **Window**: Top-level windows with title, size, position
- **VBox**: Vertical stack layout with spacing and borders
- **HBox**: Horizontal flow layout with spacing and borders
- **Frame**: Grouped controls with labeled borders
- **Tab/TabPage**: Multi-page tabbed interfaces
- **Space**: Flexible spacing element for alignment

### Data Views
- **List**: Multi-column data grids with headers, checkboxes, icons
- **ListColumn**: Column definitions with width and alignment
- **Tree**: Hierarchical tree views with icons and checkboxes
- **Calendar**: Date selection widget

### Menu & Toolbar
- **Menu**: Menu bar and popup menus
- **MenuItem**: Menu items with submenus, separators, check/radio items
- **Toolbar**: Application toolbars
- **ToolbarButton**: Standard toolbar buttons
- **ToolbarCheckButton**: Toggle toolbar buttons
- **ToolbarSeparator**: Visual toolbar separators
- **ToolbarRadioGroup**: Grouped radio buttons in toolbar

### Display Widgets
- **ImageView**: Image display with scrolling and alignment

## Common Properties Demonstrated

### Layout Properties
- `expand`: Controls widget expansion ratio (0-1 or higher integers)
- `space`: Border spacing in pixels for containers
- `border`: Enable/disable container borders

### Widget Properties
- `enabled`: Enable/disable user interaction
- `visible`: Show/hide widgets
- `tooltip-text`: Hover tooltips
- `alignment`: Text/content alignment (left, center, right, top, bottom, etc.)

### Window Properties
- `size`: Window dimensions "width height"
- `position`: Window placement "x y" or "center"
- `resizable`: Allow window resizing
- `border`: Show/hide window decorations
- `status-text`: Status bar text
- `icon`: Window icon path

### Data Properties
- `model`: Reference to data model ID
- `stylemanager`: Reference to style manager ID

## Event Types Shown

### Common Events
- `onclick`: Button and menu clicks
- `onchange`: Value changes (slider, entry, etc.)
- `onsubmit`: Form submission (entry fields)
- `onselect`: List/tree/tab selection
- `onclose`: Window close event

### Mouse Events
- `onmousedown`, `onmouseup`, `onmouseclick`
- `onmousedblclick`, `onmousemove`
- `onmouseover`, `onmouseout`

### Keyboard Events
- `onkeydown`, `onkeyup`, `onkeypress`

### Focus Events
- `onfocus`, `onblur`

### State Events
- `onshow`, `onhide`
- `onenable`, `ondisable`
- `onresize`, `onmove`

## Usage

To use these examples:

1. Load any JSON file with the XGUI loader
2. Examine the structure to understand widget hierarchy
3. Modify properties to experiment with different configurations
4. Combine elements from different examples to create custom interfaces

## Learning Path

### Beginners
Start with examples 01-05 to understand basic widgets and simple layouts.

### Intermediate Users
Progress to examples 06-10 to learn about forms, tabs, and combined layouts.

### Advanced Users
Study examples 11-15 for complex applications with menus, toolbars, and data views.

### Reference
Use examples 16-18 as reference for specific features like layouts, radio groups, and comprehensive widget showcases.

## File Naming Convention

- **01-05**: Simple examples (2-3 widgets)
- **06-10**: Medium complexity (multiple features)
- **11-15**: Complex applications (full featured)
- **16-18**: Specialized demonstrations

## Using Models in C++

Examples 11, 12, 14, 15, and 18 use data models for List, Tree, and Combobox widgets. These models must be created and populated programmatically. Here's a basic example:

```cpp
#include <xgui/xgui.h>

int main() {
    // Create a model
    xgui::Model* model = xgui::Master::CreateModel();

    // Add data to the model
    std::vector<std::string> row1;
    row1.push_back("John Doe");
    row1.push_back("john@example.com");
    model->appendChild(row1);

    std::vector<std::string> row2;
    row2.push_back("Jane Smith");
    row2.push_back("jane@example.com");
    model->appendChild(row2);

    // Load the JSON GUI
    xgui::Object* gui = xgui::Master::LoadJson("11-data-list.json", 0);
    xgui::Window* window = dynamic_cast<xgui::Window*>(gui);

    // Find the list widget and assign the model
    xgui::List* list = dynamic_cast<xgui::List*>(
        window->findChild("contactList")
    );
    if (list) {
        list->setModel(model);
    }

    window->show();
    xgui::Master::Run();

    return 0;
}
```

For complete examples of model usage, see:
- `/tests/exe_test/main.cpp` - Basic model creation and usage
- `/tests/plan/main.cpp` - Advanced model manipulation
- `/output/model.json` - Model JSON structure reference

## Additional Resources

- See `/docs` directory for complete API documentation
- Check `/tests/exe_test/main.cpp` for programmatic examples
- Review widget implementation in `/xgui/*.cpp` for detailed behavior

## Tips

1. **Start Simple**: Begin with basic examples and progressively add complexity
2. **Use IDs**: Always assign meaningful `id` attributes for widget access
3. **Layout First**: Plan your container hierarchy (VBox/HBox) before adding widgets
4. **Expand Property**: Master the `expand` property for proper widget sizing
5. **Frames**: Use frames to group related controls visually
6. **Tabs**: Use tabs to organize complex interfaces into logical sections
7. **Models**: Use data models for lists, trees, and comboboxes for dynamic content
8. **Events**: Plan which events you need before implementing handlers

## Contributing

When adding new examples:
- Follow the naming convention (number-description.json)
- Update this README with widget list and description
- Ensure JSON is properly formatted and validated
- Test the example works correctly
- Add appropriate complexity level
- Document any special features demonstrated

# Complex GUI Test - Python Bindings

This directory contains a comprehensive test example demonstrating the xgui Python bindings with a complex, interactive GUI loaded from JSON.

## Files

- **`complex_gui.json`** - JSON definition of a complex multi-tab GUI with various interactive widgets
- **`test_complex_gui.py`** - Python script that loads the GUI and binds all interactive events
- **`README.md`** - This file

## Features Demonstrated

### 1. GUI Creation
- **Currently**: Creates GUI programmatically using `xgui.Master.Create*()` methods
- **Future**: A JSON file (`complex_gui.json`) is provided for when `LoadJson()` is available in Python bindings
- Multi-level widget hierarchy with tabs, frames, and containers
- Multiple widget types: sliders, progress bars, entries, labels, buttons, checkboxes, comboboxes

**Note**: The `xgui.Master.LoadJson()` method exists in C++ but is not yet exposed to Python bindings.
The SWIG interface file has been updated (`pywrap/master.i`) and will be available after the next rebuild.

### 2. Event Binding
- Demonstrates binding multiple event types: `onclick`, `onchange`, `onclose`
- Shows proper callback creation using `xgui.PyCallback()`
- Widget event linking with `widget.linkEvent()`

### 3. Interactive Widget Behaviors

#### Tab 1: Sliders & Progress
- **Volume Control**: Slider updates progress bar and label in real-time
- **Brightness Control**: Slider and spin widget are synchronized (bidirectional)
- **Download Simulator**: Button-triggered progress with incremental updates

#### Tab 2: Input Controls
- **Text Mirror**: Entry field changes are mirrored to a label
- **Character Counter**: Entry tracks character count with progress bar visualization
- **Widget Enable/Disable**: Checkbox controls editable state of other widgets

#### Tab 3: Selection Controls
- **Theme Selector**: Combobox selection updates display label
- **Service Checkboxes**: Multiple checkboxes update a status summary label

### 4. Cross-Widget Communication
Examples of widgets affecting other widgets:
- Slider position → Progress bar position + Label text
- Slider position ↔ Spin value (bidirectional sync)
- Entry text → Label text (mirror)
- Entry text → Character count + Progress bar
- Checkbox state → Entry editable state
- Multiple checkboxes → Combined status label

## Requirements

- xgui library built with Python bindings
- Python 3.x
- X11 or Wayland display server (graphical environment)
- `DISPLAY` environment variable set

## Important Note

The test currently creates the GUI programmatically because `xgui.Master.LoadJson()` is not yet available in the Python bindings. The SWIG interface file (`pywrap/master.i`) has been updated to include LoadJson, but the bindings need to be rebuilt with:

```bash
make python
```

Once rebuilt, you can modify the test to use `xgui.Master.LoadJson("complex_gui.json")` instead of programmatic creation.

## Running the Test

### From the tests/python_tests directory:

```bash
# Make the script executable
chmod +x test_complex_gui.py

# Run the test
python3 test_complex_gui.py
```

### From the repository root:

```bash
python3 tests/python_tests/test_complex_gui.py
```

## Expected Output

The script will:
1. Print startup information
2. Load the GUI from JSON
3. Bind all event callbacks
4. Display the interactive window
5. Print event notifications to the console as you interact with widgets
6. Clean up and exit when you close the window or click Quit

### Console Output Example:

```
============================================================
Complex GUI Test - xgui Python Bindings
============================================================

Loading GUI from: /path/to/complex_gui.json
✓ GUI loaded successfully

Binding events to widgets...
  ✓ Bound volume slider
  ✓ Bound brightness slider
  ✓ Bound brightness spin
  ✓ Bound start download button
  ✓ Bound reset button
  ✓ Bound text entry
  ✓ Bound counter entry
  ✓ Bound enable checkbox
  ✓ Bound apply button
  ✓ Bound theme combobox
  ✓ Bound WiFi checkbox
  ✓ Bound Bluetooth checkbox
  ✓ Bound GPS checkbox
  ✓ Bound quit button
  ✓ Bound window close event

All events bound successfully!

Showing window and starting event loop...

Interactive features:
  - Volume slider controls progress bar
  - Brightness slider and spin are synchronized
  - Download simulator with start/reset buttons
  - Text entry mirrors to label
  - Character counter with progress bar
  - Enable checkbox controls widget states
  - Theme selector updates display
  - Service checkboxes update status

Close the window or click Quit to exit.

Volume changed to: 75%
Brightness slider changed to: 128
Text entry changed to: Hello xgui!
Character count: 15
...
```

## Code Structure

### Main Class: `ComplexGUITest`

The test is organized as a class with the following structure:

```python
class ComplexGUITest:
    def __init__(self):
        self.win = None              # Main window
        self.widgets = {}            # Widget cache
        self.callbacks = {}          # Callback storage
        self.download_progress = 0   # State tracking

    def get_widget(self, widget_id):
        """Get widget by ID with caching"""

    # Event callback methods
    def on_volume_change(self, widget):
        """Handle volume slider changes"""

    def on_brightness_slider_change(self, widget):
        """Handle brightness slider changes"""

    # ... more callbacks ...

    def create_callback(self, func):
        """Helper to create PyCallback objects"""

    def bind_events(self):
        """Bind all event callbacks"""

    def run(self):
        """Main application logic"""
```

### Key Patterns

#### Storing and Getting Widgets
Since widgets are created programmatically, we store references in a dictionary:

```python
# Store a widget with a name
def store_widget(self, name, widget):
    self.widgets[name] = widget
    widget.set("id", name)  # Also set the id property
    return widget

# Get a stored widget
def get_widget(self, widget_name):
    return self.widgets.get(widget_name)
```

Alternatively, when using LoadJson/LoadXml, widgets can be found using the Container.findChild() method:
```python
# On a container (window, vbox, etc.)
widget = container.findChild("myWidgetId")
```

#### Reading Widget Properties
```python
value = widget.get("property_name")
# Examples:
text = entry.get("text")
pos = slider.get("pos")
checked = checkbox.get("checked")
```

#### Setting Widget Properties
```python
widget.set("property_name", "value")
# Examples:
label.set("text", "New Text")
slider.set("pos", "50")
progress.set("pos", str(value))
```

#### Creating and Binding Callbacks

There are different callback types depending on the widget and event:

**PyCallback** - For buttons, checkboxes, sliders, etc. (receives widget only):
```python
def my_callback(widget):
    # Do something
    return xgui.EVT_BLOCK

callback = xgui.PyCallback(my_callback)
widget.linkEvent("onclick", callback)
```

**PyTextCallback** - For Entry widgets (receives widget and text):
```python
def entry_callback(widget, text):
    # Handle text change
    print(f"Text changed to: {text}")
    return xgui.EVT_BLOCK

callback = xgui.PyTextCallback(entry_callback)
entry.linkEvent("onchange", callback)
```

**Other callback types**:
- `PyTextStatusCallback(func)` - receives (widget, text, status)
- `PyTextPairCallback(func)` - receives (widget, text1, text2)

## Widget Properties Reference

### Common Properties
- `text` - Text content or label
- `pos` - Position/value for sliders, spins, progress bars
- `min` / `max` - Range for numeric widgets
- `checked` - Checkbox state ("0" or "1")
- `editable` - Entry field editable state ("0" or "1")

### Common Events
- `onclick` - Button clicks
- `onchange` - Value/text changes (entries, sliders, checkboxes, etc.)
- `onclose` - Window close event
- `onselect` - Selection events (lists, trees)

## Extending the Test

To add more interactive behaviors:

1. **Add widgets to JSON**: Add new widget definitions with unique `id` attributes
2. **Create callback method**: Add a new method to handle the event
3. **Bind the event**: Call `widget.linkEvent()` in `bind_events()`
4. **Update other widgets**: Use `get_widget()` and `set()` to modify related widgets

### Example: Adding a New Slider

```python
# In JSON:
{
  "type": "slider",
  "id": "mySlider",
  "min": "0",
  "max": "100",
  "pos": "0"
}

# In Python:
def on_my_slider_change(self, widget):
    value = int(widget.get("pos"))
    # Do something with value
    return xgui.EVT_BLOCK

def bind_events(self):
    # ... existing bindings ...
    my_slider = self.get_widget("mySlider")
    if my_slider:
        my_slider.linkEvent("onchange", self.create_callback(self.on_my_slider_change))
```

## Troubleshooting

### Import Error
If you get `ImportError: No module named 'xgui'`:
- Ensure xgui is built with Python bindings enabled
- Check that `output/xgui.py` and `output/_xgui.so` exist
- Verify the path in `sys.path.insert(0, ...)` is correct

### Display Error
If you get display-related errors:
- Ensure `DISPLAY` environment variable is set
- Verify X11 or Wayland is running
- Try: `export DISPLAY=:0`

### Widget Not Found
If widgets cannot be found by ID:
- Verify the widget has an `id` attribute in the JSON
- Check for typos in widget IDs
- Ensure the JSON loaded successfully

### Events Not Firing
If callbacks aren't being called:
- Verify the callback returns `xgui.EVT_BLOCK`
- Check that the callback is wrapped in `xgui.PyCallback()`
- Ensure `linkEvent()` was called successfully
- Verify the event name matches the widget type (e.g., "onclick" for buttons)

## Learning Resources

- See `../../examples/` for more JSON GUI examples
- See `../python_test/gui_example_python3.py` for programmatic GUI creation
- See `../python_test/test_python3.py` for basic Python binding tests

## Contributing

When adding new features to this test:
- Follow the existing callback naming convention: `on_<widget>_<event>`
- Add comments explaining complex interactions
- Update this README with new features
- Keep the code well-organized and readable

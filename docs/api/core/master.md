# Master

The `Master` class is the main entry point for XGUI applications. It derives from `Object` and provides static factory methods for creating and managing all XGUI objects, including widgets, windows, models, and style managers.

## Class Hierarchy

```
Object
└── Master
```

## Methods

### Instance Management

#### `xguimpl::Master * getImpl()`

Returns a pointer to the Master implementation object.

**Returns:** Pointer to the platform-specific Master implementation.

---

#### `static Master * Instance()`

Returns the current instance of the Master class or creates a new instance if it does not exist yet.

**Returns:** Pointer to the Master singleton instance.

---

### Event Loop

#### `static void Quit()`

Exits the XGUI main event loop started with the `Run()` method.

---

#### `static int Run()`

Enters the main XGUI event loop.

**Returns:** Exit code from the event loop.

---

#### `static void RegisterIdleCallback(xgui::Callback * cbk, xgui::Widget * wid)`

Registers a callback function that will be executed after the main event loop has finished processing currently pending events.

**Parameters:**
- `cbk` - Pointer to the Callback object to execute
- `wid` - Widget that will be passed as a parameter to the callback function

---

### Context Management

#### `static xgui::Container * FindContext(std::string const &id)`

Returns the context (usually a top-level window) with the specified id.

**Parameters:**
- `id` - The identifier of the context to find

**Returns:** Pointer to the Container context, or `NULL` if not found.

---

#### `static void RegisterContext(xgui::Container * w)`

Registers a new context in the internal contexts list.

**Parameters:**
- `w` - Pointer to the Container to register as a context

---

#### `static void UnregisterContext(xgui::Container * w)`

Removes a context from the internal contexts list.

**Parameters:**
- `w` - Pointer to the Container to unregister

---

### Image Management

#### `static xgui::Image * LoadImage(std::string const &path)`

Loads an image from the specified file and returns an `xgui::Image` object that represents it. If the specified file has already been loaded then the same Image object is returned.

**Parameters:**
- `path` - Path to the image file to load

**Returns:** Pointer to the Image object.

---

#### `static xgui::Image * GetImage(std::string const &path)`

Returns a pointer to an Image object of an already loaded image, or NULL if the specified image file is not loaded.

**Parameters:**
- `path` - Path to the image file

**Returns:** Pointer to the Image object, or `NULL` if not loaded.

---

#### `static xgui::Image * NullImage()`

Returns a pointer to an Image object that does not represent a real image. It is used when a function argument requires an Image object reference.

**Returns:** Pointer to a null Image object.

---

### Threading

#### `static void InitThreads()`

Initializes the XGUI threading environment. This function should be called on application startup.

---

#### `static void QuitThreads()`

Terminates the XGUI threading environment. This function should be called on application cleanup.

---

#### `static void GuiLock()`

Locks the GUI for thread-safe operations.

---

#### `static void GuiUnlock()`

Unlocks the GUI after thread-safe operations.

---

### Widget Factory Methods

#### `static xgui::Window * CreateWindow(xgui::Window * parent = 0)`

Creates a new Window widget and returns a pointer to it. If `parent` is specified then a modal window is created.

**Parameters:**
- `parent` - Optional pointer to parent Window for creating modal windows (default: `0`)

**Returns:** Pointer to the newly created Window.

---

#### `static xgui::Button * CreateButton(xgui::Container * parent, std::string const &text, xgui::Image * image = 0, bool toggle_button = false)`

Creates a new Button widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `text` - The text displayed on the button
- `image` - Optional pointer to Image displayed on the button (default: `0`)
- `toggle_button` - If `true`, creates a toggle button that maintains its pressed/unpressed state (default: `false`)

**Returns:** Pointer to the newly created Button.

---

#### `static xgui::Menu * CreateMenu(xgui::Window * parent, bool visible = true)`

Creates a new Menu widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Window
- `visible` - If `false`, the menu is initially hidden (default: `true`)

**Returns:** Pointer to the newly created Menu.

---

#### `static xgui::VBox * CreateVBox(xgui::Container * parent, int space = 0, bool border = false)`

Creates a new VBox widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `space` - Space in pixels to be left as a margin (default: `0`)
- `border` - If `true`, a 3D border is displayed around the widget (default: `false`)

**Returns:** Pointer to the newly created VBox.

---

#### `static xgui::HBox * CreateHBox(xgui::Container * parent, int space = 0, bool border = false)`

Creates a new HBox widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `space` - Space in pixels to be left as a margin (default: `0`)
- `border` - If `true`, a 3D border is displayed around the widget (default: `false`)

**Returns:** Pointer to the newly created HBox.

---

#### `static xgui::Entry * CreateEntry(xgui::Container * parent, std::string const &text, int maxlen = 0, bool editable = true, bool password_mode = false)`

Creates a new Entry widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `text` - The text initially displayed in the entry
- `maxlen` - Maximum length in characters of inserted text (default: `0` for unlimited)
- `editable` - If `false`, the text is read-only (default: `true`)
- `password_mode` - If `true`, an asterisk is displayed for each character (default: `false`)

**Returns:** Pointer to the newly created Entry.

---

#### `static xgui::Label * CreateLabel(xgui::Container * parent, std::string const &text)`

Creates a new Label widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `text` - The text displayed in the label

**Returns:** Pointer to the newly created Label.

---

#### `static xgui::List * CreateList(xgui::Container * parent, xgui::StyleManager * sm = 0)`

Creates a new List widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `sm` - Optional pointer to a StyleManager object that defines the visual styles of list items (default: `0`)

**Returns:** Pointer to the newly created List.

---

#### `static xgui::Tree * CreateTree(xgui::Container * parent, xgui::StyleManager * sm = 0)`

Creates a new Tree widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `sm` - Optional pointer to a StyleManager object that defines the visual styles of tree items (default: `0`)

**Returns:** Pointer to the newly created Tree.

---

#### `static xgui::Slider * CreateSlider(xgui::Container * parent, int min = 0, int max = 100, bool vertical = false)`

Creates a new Slider widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `min` - Minimum position of the slider (default: `0`)
- `max` - Maximum position of the slider (default: `100`)
- `vertical` - If `true`, the slider moves vertically; otherwise horizontally (default: `false`)

**Returns:** Pointer to the newly created Slider.

---

#### `static xgui::Calendar * CreateCalendar(xgui::Container * parent)`

Creates a new Calendar widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container

**Returns:** Pointer to the newly created Calendar.

---

#### `static xgui::ImageView * CreateImageView(xgui::Container * parent, xgui::Image * img = 0)`

Creates a new ImageView widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `img` - The image to be displayed in the ImageView (default: `0`)

**Returns:** Pointer to the newly created ImageView.

---

#### `static xgui::Progressbar * CreateProgressbar(xgui::Container * parent, int min = 0, int max = 100)`

Creates a new Progressbar widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `min` - Minimum progressbar counter value (default: `0`)
- `max` - Maximum progressbar counter value (default: `100`)

**Returns:** Pointer to the newly created Progressbar.

---

#### `static xgui::Tab * CreateTab(xgui::Container * parent, const std::string & alignment = "top")`

Creates a new Tab widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `alignment` - Specifies where the tab labels are displayed: `"top"`, `"left"`, `"right"`, or `"bottom"` (default: `"top"`)

**Returns:** Pointer to the newly created Tab.

---

#### `static xgui::Checkbox * CreateCheckbox(xgui::Container * parent, std::string const &text)`

Creates a new Checkbox widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `text` - The text displayed near the checkbox

**Returns:** Pointer to the newly created Checkbox.

---

#### `static xgui::Spin * CreateSpin(xgui::Container * parent, int min = 0, int max = 100)`

Creates a new Spin widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `min` - Minimum value (default: `0`)
- `max` - Maximum value (default: `100`)

**Returns:** Pointer to the newly created Spin.

---

#### `static xgui::Frame * CreateFrame(xgui::Container * parent, const std::string & label)`

Creates a new Frame widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `label` - The text displayed on the frame top border

**Returns:** Pointer to the newly created Frame.

---

#### `static xgui::Combobox * CreateCombobox(xgui::Container * parent, bool editable = false)`

Creates a new Combobox widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `editable` - If `true`, the entry part of the combobox can be manually edited (default: `false`)

**Returns:** Pointer to the newly created Combobox.

---

#### `static xgui::Space * CreateSpace(xgui::Container * parent, int min_width = 1, int min_height = 1)`

Creates a new Space widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container
- `min_width` - Minimum width of the space widget (default: `1`)
- `min_height` - Minimum height of the space widget (default: `1`)

**Returns:** Pointer to the newly created Space.

---

#### `static xgui::Toolbar * CreateToolbar(xgui::Container * parent)`

Creates a new Toolbar widget and returns a pointer to it. The newly created widget is inserted as a child of the `parent` container.

**Parameters:**
- `parent` - Pointer to the parent Container

**Returns:** Pointer to the newly created Toolbar.

---

### Widget Destruction

#### `static void DestroyWidget(xgui::Widget * w)`

Destroys the specified widget, removing it from its parent container.

**Parameters:**
- `w` - Pointer to the Widget to destroy

---

### Model Management

#### `static xgui::Model * CreateModel()`

Creates a new model to be used in widgets derived from the View class.

**Returns:** Pointer to the newly created Model.

---

#### `static void DestroyModel(xgui::Model * model)`

Destroys the specified model. If the model is assigned to some views, it's not destroyed.

**Parameters:**
- `model` - Pointer to the Model to destroy

---

### StyleManager Management

#### `static xgui::StyleManager * CreateStyleManager()`

Creates a new StyleManager object that is responsible for holding the visual styles of styled items.

**Returns:** Pointer to the newly created StyleManager.

---

#### `static void DestroyStyleManager(xgui::StyleManager * sm)`

Destroys a StyleManager object.

**Parameters:**
- `sm` - Pointer to the StyleManager to destroy

---

### Dialog Methods

#### `static xgui::StrVector OpenFileDialog(xgui::Window * parent, const xgui::FileExtensionsVector &filter, std::string const &title, std::string const &path = "", bool multiselection = false)`

Opens a common requester for opening a file.

**Parameters:**
- `parent` - The window over which the dialog will be displayed; can be `NULL`
- `filter` - Vector of `xgui::FileExtension` structures that specifies the list of file types that the dialog will display
- `title` - The dialog window title
- `path` - Optional path representing the directory in which the dialog opens (default: `""`)
- `multiselection` - If `true`, allows selection of multiple files (default: `false`)

**Returns:** Vector of strings representing the list of selected files with full paths.

---

#### `static std::string SaveFileDialog(xgui::Window * parent, const xgui::FileExtensionsVector &filter, std::string const &title, std::string const &path = "", std::string const &file_name = "")`

Opens a common requester for saving a file.

**Parameters:**
- `parent` - The window over which the dialog will be displayed; can be `NULL`
- `filter` - Vector of `xgui::FileExtension` structures that specifies the list of file types that the dialog will display
- `title` - The dialog window title
- `path` - Optional path representing the directory in which the dialog opens (default: `""`)
- `file_name` - Optional name of a file initially proposed by the dialog as a save file name (default: `""`)

**Returns:** The file name with full path of the selected file.

---

#### `static int MessageDialog(xgui::Window * parent, const std::string & title, const std::string & text, const std::string & type = "info", const std::string & buttons = "ok")`

Opens a common dialog for displaying a message text.

**Parameters:**
- `parent` - The window over which the dialog will be displayed; can be `NULL`
- `title` - The dialog window title
- `text` - The text that the dialog should display
- `type` - The message severity class: `"warn"`, `"question"`, `"err"`, or `"info"` (default: `"info"`)
- `buttons` - Determines what buttons are displayed: `"ok"`, `"ok-cancel"`, `"yes-no"`, or `"yes-no-cancel"` (default: `"ok"`)

**Returns:** Integer representing which button was clicked.

---

### Dynamic Class System

#### `static void InitDynamicClasses()`

Initializes the dynamic class environment.

---

#### `static xgui::Object * LoadXml(std::string const &filename, xgui::Object * parent = 0)`

Loads an XML file representing a structure of XGUI widgets and returns the root object of the hierarchy.

**Parameters:**
- `filename` - Path to the XML file to load
- `parent` - The object that will contain the loaded object (default: `0`)

**Returns:** Pointer to the root object of the loaded hierarchy.

---

#### `static void SaveXml(xgui::Object * root, std::string const &filename)`

Saves the `root` object in an XML structured file.

**Parameters:**
- `root` - Pointer to the root object to save
- `filename` - Path to the XML file to create

---

#### `static void LoadLibrary(std::string const &filename)`

Loads an external plugin containing XGUI widgets.

**Parameters:**
- `filename` - Path to the library file to load

---

#### `static void UnloadLibrary(std::string const &filename)`

Unloads an external library previously loaded with the `LoadLibrary()` method.

**Parameters:**
- `filename` - Path to the library file to unload

---

#### `static void RegisterClass(xgui::ClassInfo * ginfo)`

Registers a new widget described by its ClassInfo class.

**Parameters:**
- `ginfo` - Pointer to the ClassInfo structure describing the widget class

---

#### `static void UnregisterClass(std::string const &cname)`

Unregisters the widget with the specified class name.

**Parameters:**
- `cname` - The class name to unregister

---

#### `static xgui::Object * CreateObject(std::string const &class_name, xgui::Object * parent, xgui::StringMap const &properties)`

Creates an XGUI object given its class name.

**Parameters:**
- `class_name` - The name of the class to instantiate
- `parent` - The object that will contain the newly created object
- `properties` - Set of properties assigned to the object immediately after creation

**Returns:** Pointer to the newly created Object.

---

#### `static xgui::Object * FinalizeObject(xgui::Object * o)`

Finalizes object creation.

**Parameters:**
- `o` - Pointer to the Object to finalize

**Returns:** Pointer to the finalized Object.

---

#### `static void DestroyObject(xgui::Object * o)`

Destroys an object.

**Parameters:**
- `o` - Pointer to the Object to destroy

---

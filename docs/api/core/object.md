# Object

The `Object` class is the base class for all XGUI objects. It provides fundamental functionality for object identification, property management, dynamic method calls, and object lifecycle management.

## Class Hierarchy

```
Object
```

## Methods

### Class Information

#### `std::string const &className()`

Returns the name of the class this object belongs to.

**Returns:** The class name as a string.

---

#### `xgui::ClassInfo * getClass()`

Returns a pointer to a ClassInfo structure that represents the object class.

**Returns:** Pointer to ClassInfo structure.

---

#### `bool isWidget()`

Returns true if the object is a widget (i.e., it's castable to Widget class), false otherwise.

**Returns:** `true` if object is a Widget, `false` otherwise.

---

#### `bool isModel()`

Returns true if the object is a model (i.e., it's castable to Model class), false otherwise.

**Returns:** `true` if object is a Model, `false` otherwise.

---

#### `bool isStyleManager()`

Returns true if the object is a style manager (i.e., it's castable to StyleManager class), false otherwise.

**Returns:** `true` if object is a StyleManager, `false` otherwise.

---

#### `bool isContainer()`

Returns true if the object is a container (i.e., it's castable to Container class), false otherwise.

**Returns:** `true` if object is a Container, `false` otherwise.

---

### Object Identification

#### `bool isIdentifiedAs(std::string const &id)`

Returns true if the object's id is equal to the specified id; false otherwise.

**Parameters:**
- `id` - The identifier to check against

**Returns:** `true` if the object has the specified ID, `false` otherwise.

---

#### `std::string const &id()`

Returns the object's id.

**Returns:** The object's identifier as a string.

---

#### `void setId(std::string const &id)`

Sets a new id for the object.

**Parameters:**
- `id` - The new identifier for the object

---

### Object Storage

#### `void store(std::string const &name, xgui::Object * o, bool destroy = false)`

Stores a reference to an object `o` within this object. `name` is a string used for retrieving the object reference later.

If `destroy` is set to true, then the object `o` is explicitly destroyed (i.e., passed to the `Master::Destroy()` function) when removed from the store.

**Parameters:**
- `name` - The key name for storing the object reference
- `o` - Pointer to the object to store
- `destroy` - If `true`, the object will be destroyed when removed (default: `false`)

---

#### `xgui::Object * retrieve(std::string const &name)`

Returns an object reference previously set with the `store()` method.

**Parameters:**
- `name` - The key name of the stored object

**Returns:** Pointer to the stored object, or `NULL` if not found.

---

#### `void unstore(std::string const &name)`

Removes an object reference previously set with the `store()` method.

**Parameters:**
- `name` - The key name of the stored object to remove

---

### Property Management

#### `bool set(std::string const &name, std::string const &val)`

Sets the property specified by `name` to the value `val`.

**Parameters:**
- `name` - The property name
- `val` - The property value as a string

**Returns:** `true` if the property was set successfully, `false` otherwise.

---

#### `bool get(std::string const &name, std::string &dest)`

Gets the property specified by `name`, filling the `dest` string with its value.

**Parameters:**
- `name` - The property name
- `dest` - Reference to string that will receive the property value

**Returns:** `true` if the property exists, `false` otherwise.

---

#### `void registerProperty(std::string const &prop, xgui::PropertySetter setter, xgui::PropertyGetter getter)`

Registers a new property handler for the object.

**Parameters:**
- `prop` - The property name
- `setter` - Pointer to the function that will be called for setting the property value
- `getter` - Pointer to the function that will be called for retrieving the property value

---

#### `std::vector<std::string> properties()`

Returns a vector of all property names of the object.

**Returns:** Vector containing all property names.

---

#### `xgui::PropertyType hasProperty(std::string const &property)`

Checks if the object has the specified property and returns its type.

**Parameters:**
- `property` - The property name to check

**Returns:**
- `xgui::PROPERTY_NOT_FOUND` - if the object doesn't have the specified property
- `xgui::PROPERTY_READ_ONLY` - if the property is read-only
- `xgui::PROPERTY_READ_WRITE` - if the property is read-write

---

### Ownership Management

#### `virtual void addOwner(void *o)`

Adds `o` among the object owners.

**Parameters:**
- `o` - Pointer to the owner object

---

#### `virtual void delOwner(void *o)`

Removes `o` from the object owners.

**Parameters:**
- `o` - Pointer to the owner object to remove

---

#### `virtual void setOwnerDestructionNotifier(void (*notifier)(void *, xgui::Object *))`

Sets a function that will be called for each registered owner when the object is destroyed.

**Parameters:**
- `notifier` - Function pointer that will be called on destruction

---

### Serialization

#### `std::string pack()`

Returns a string representing the object.

**Returns:** String representation of the object.

---

#### `static xgui::Object * Unpack(std::string const &obj)`

Returns an object given a pack string.

**Parameters:**
- `obj` - The packed string representation

**Returns:** Pointer to the unpacked object.

---

### Dynamic Methods

#### `DMethod * findMethod(std::string const &name)`

Returns the dynamic method called `name` or NULL if the object doesn't have one.

**Parameters:**
- `name` - The method name to find

**Returns:** Pointer to the DMethod object, or `NULL` if not found.

---

#### `void registerMethod(std::string const &name, DMethod * m)`

Registers a new dynamic method.

**Parameters:**
- `name` - The method name
- `m` - Reference to an object that encapsulates the method

---

#### `std::string call(std::string const &method, std::string const &p1)`

Calls a dynamic method with one parameter.

**Parameters:**
- `method` - The method name
- `p1` - First parameter

**Returns:** The method result as a string.

---

#### `std::string call(std::string const &method, std::string const &p1, std::string const &p2)`

Calls a dynamic method with two parameters.

**Parameters:**
- `method` - The method name
- `p1` - First parameter
- `p2` - Second parameter

**Returns:** The method result as a string.

---

#### `std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3)`

Calls a dynamic method with three parameters.

**Parameters:**
- `method` - The method name
- `p1` - First parameter
- `p2` - Second parameter
- `p3` - Third parameter

**Returns:** The method result as a string.

---

#### `std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4)`

Calls a dynamic method with four parameters.

**Parameters:**
- `method` - The method name
- `p1` - First parameter
- `p2` - Second parameter
- `p3` - Third parameter
- `p4` - Fourth parameter

**Returns:** The method result as a string.

---

#### `std::string call(std::string const &method, std::string const &p1, std::string const &p2, std::string const &p3, std::string const &p4, std::string const &p5)`

Calls a dynamic method with five parameters.

**Parameters:**
- `method` - The method name
- `p1` - First parameter
- `p2` - Second parameter
- `p3` - Third parameter
- `p4` - Fourth parameter
- `p5` - Fifth parameter

**Returns:** The method result as a string.

---

## Properties

### `id` (Read/Write)

Sets or retrieves the object ID.

**Type:** String

**Example:**
```cpp
object->set("id", "myObject");

std::string objectId;
object->get("id", objectId);
```

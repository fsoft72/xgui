# Image

The `Image` class represents an image loaded from a file. It derives from `Object` and provides methods for accessing image properties such as dimensions and file path. This class cannot be directly instantiated, but must be created using the `Master::LoadImage()` function.

## Class Hierarchy

```
Object
└── Image
```

## Methods

### Reference Counting

#### `int ref()`

Increments the reference count of the image object and returns the incremented value.

**Returns:** The incremented reference count.

---

#### `int unref()`

Decrements the reference count of the image object and destroys the object when it reaches 0. Returns the decremented value.

**Returns:** The decremented reference count.

---

### Image Properties

#### `std::string const &getPath()`

Returns the path of the file containing the image that this object represents.

**Returns:** The file path as a string.

---

#### `int getWidth()`

Returns the width of the image in pixels.

**Returns:** The image width in pixels.

---

#### `int getHeight()`

Returns the height of the image in pixels.

**Returns:** The image height in pixels.

---

### Implementation

#### `xguimpl::Image * getImpl()`

Returns a reference to the implementation class.

**Returns:** Pointer to the platform-specific Image implementation.

---

### Validation

#### `bool isValid()`

Returns true if this object represents a valid image; false otherwise.

**Returns:** `true` if the image is valid, `false` otherwise.

---

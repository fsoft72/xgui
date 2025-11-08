# ImageView

The `ImageView` widget displays an image with support for scrolling or automatic resizing. It derives from `Widget` and provides methods for managing the displayed image and view settings.

## Class Hierarchy

```
Object
└── Widget
    └── ImageView
```

## Methods

### Image Management

#### `xgui::Image * getImage()`

Returns the image that the ImageView widget is currently displaying.

**Returns:** Pointer to the currently displayed Image, or `NULL` if no image is set.

---

#### `void setImage(xgui::Image * img)`

Sets a new image to display in the ImageView widget.

**Parameters:**
- `img` - Pointer to the Image to display

---

### View Properties

#### `std::pair<int, int> getViewSize()`

Returns a pair of integers representing the view size of the image. The view size represents the visible portion of the image.

**Returns:** Pair of integers containing width and height of the view.

---

#### `bool isScrolled()`

Returns true if the image is scrolled, false if the image is resized to fit the view size.

**Returns:** `true` if scrolled, `false` if resized to fit.

**See also:** `scrolled` property.

---

## Properties

### `scrolled` (Read/Write)

Controls whether the image is scrolled or resized to fit.

**Values:**
- `"1"` - Image is scrolled; only the portion specified by `view-size` is displayed and scrollbars are used to scroll
- `"0"` - Image is resized to fit the dimensions specified by `view-size`

**Default:** `"0"`

---

### `view-size` (Read/Write)

Sets or retrieves the view size. The view size represents the visible portion of the image.

**Type:** String in format `"width height"`

---

### `image-path` (Read/Write)

Sets or retrieves the path of the file containing the currently displayed image.

**Type:** String

---

### `alignment` (Read/Write)

Sets or retrieves the image alignment in the imageview widget.

**Values:**
- `"top-left"` - Align to top-left corner
- `"top"` - Align to top center
- `"top-right"` - Align to top-right corner
- `"left"` - Align to left center
- `"center"` - Center alignment
- `"right"` - Align to right center
- `"bottom-left"` - Align to bottom-left corner
- `"bottom"` - Align to bottom center
- `"bottom-right"` - Align to bottom-right corner

**Default:** `"center"`

---

## Events

This widget has no events beyond those inherited from `Widget`.

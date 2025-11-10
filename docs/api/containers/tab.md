# Tab

The `Tab` widget provides a tabbed container where each child widget is displayed on a separate tab page. It derives from `Container` and supports page icons, labels, and page selection.

## Class Hierarchy

```
Object
└── Widget
    └── Container
        └── Tab
```

## Methods

### Page Management

#### `void setPageIcon(xgui::Widget * w, xgui::Image * icon)`

Sets the image displayed on the specified tab page near the label text.

**Parameters:**
- `w` - Pointer to the child Widget representing the page
- `icon` - Pointer to the Image to display on the tab

---

#### `void setPageLabel(xgui::Widget * w, std::string const &str)`

Sets the text displayed on the label for the specified tab page.

**Parameters:**
- `w` - Pointer to the child Widget representing the page
- `str` - The label text

---

#### `xgui::Image * getPageIcon(xgui::Widget * w)`

Returns the icon displayed on the specified tab page near the label text.

**Parameters:**
- `w` - Pointer to the child Widget representing the page

**Returns:** Pointer to the Image, or `NULL` if no icon is set.

---

#### `std::string const &getPageLabel(xgui::Widget * w)`

Returns the label text of the specified tab page.

**Parameters:**
- `w` - Pointer to the child Widget representing the page

**Returns:** The label text.

---

## Properties

### `page-count` (Read-Only)

Retrieves the number of pages that the tab widget currently holds.

**Type:** Integer (as string)

---

### `selected-page` (Read/Write)

Sets or retrieves the ID of the currently selected page of the tab.

**Type:** String

---

### `selected-index` (Read/Write)

Sets or retrieves the zero-based index of the currently selected page of the tab.

**Type:** Integer (as string)

---

## Events

### `onchange` (xgui::IntCallback)

Fired when the currently selected tab page changes.

**Parameters:**
- `page_num` - Zero-based index of the new selected page

---

## Pushed Properties

Properties that are registered on all children of this container:

### `tab-icon` (Read/Write)

Sets or retrieves the path of the file containing the icon displayed near the label text of the tab page containing this child.

**Type:** String

---

### `tab-label` (Read/Write)

Sets or retrieves the label text of the tab page containing this child.

**Type:** String

---

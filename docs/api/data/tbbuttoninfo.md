# TbButtonInfo

The `TbButtonInfo` class is an information structure used to describe a button of the Toolbar widget.

## Class Hierarchy

```
TbButtonInfo
```

## Description

This class provides a convenient way to specify toolbar button properties when working with Toolbar widgets. It encapsulates all the information needed to define a button's appearance and behavior.

## Constructors

### `TbButtonInfo()`

Initializes a new empty toolbar button with default values.

---

### `TbButtonInfo(const std::string & cname, const std::string & ctext, xgui::Image * cimage, const std::string & ctooltip)`

Initializes a new toolbar button with the specified information.

**Parameters:**
- `cname` - The name of the button (used to refer to the button in Toolbar methods)
- `ctext` - The text displayed on the button
- `cimage` - Pointer to the Image displayed on the button
- `ctooltip` - The tooltip text displayed for the button

---

## Member Variables

### `std::string name`

The name of the button, used to refer to the button in Toolbar methods.

**Type:** String

---

### `std::string text`

The text displayed on the button.

**Type:** String

---

### `xgui::Image * image`

The image displayed on the button.

**Type:** Pointer to Image

---

### `std::string tooltip`

The tooltip text displayed when hovering over the button.

**Type:** String

---

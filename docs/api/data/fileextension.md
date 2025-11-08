# FileExtension

The `FileExtension` class is an information structure used to describe a file type with its extensions. It is used in `Master::OpenFileDialog()` and `Master::SaveFileDialog()` functions.

## Class Hierarchy

```
FileExtension
```

## Description

This class provides a convenient way to specify file type filters for file dialog operations. It encapsulates the file type name and its associated extensions.

## Constructors

### `FileExtension()`

Initializes a FileExtension with just the "All files" file type.

---

### `FileExtension(std::string const &n, std::string const &ext)`

Initializes a file extension structure with the specified information.

**Parameters:**
- `n` - The file type name (e.g., `"Image files"`)
- `ext` - A semicolon-separated list of file extensions (e.g., `"*.jpg;*.png;*.bmp"`)

---

## Member Variables

### `std::string name`

The file type description.

**Type:** String

**Example:** `"Image files"`

---

### `std::string extension`

A semicolon-separated list of file extensions.

**Type:** String

**Example:** `"*.jpg;*.png;*.bmp"`

---

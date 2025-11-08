# XGUI Example Viewer

A command-line tool for loading and displaying XGUI JSON GUI definitions.

## Purpose

This tool allows you to quickly test and preview JSON-based GUI definitions without writing any C++ code. It's particularly useful for:

- Testing the example JSON files in the `examples/` directory
- Rapid prototyping of GUI layouts
- Validating JSON GUI definitions
- Learning XGUI by experimenting with different configurations

## Building

### Linux (GTK)
```bash
make -f Makefile.gtk xgui-example
```

### macOS
```bash
make -f Makefile.osx xgui-example
```

### GNUstep
```bash
make -f Makefile.gnustep xgui-example
```

The binary will be created in the `output/` directory.

## Usage

```bash
./output/xgui-example <json-file>
```

### Arguments

- `<json-file>` - Path to a JSON file containing a XGUI GUI definition

### Examples

```bash
# Test a simple button example
./output/xgui-example examples/01-simple-button.json

# Test a complex application
./output/xgui-example examples/15-multi-tab-application.json

# Test the default GUI
./output/xgui-example output/gui.json
```

## JSON File Requirements

The JSON file must contain a valid XGUI GUI definition with:

1. A top-level `window` object as the root element
2. Valid widget types and properties
3. Proper nesting of child elements

### Minimal Example

```json
{
  "type": "window",
  "text": "Hello World",
  "size": "400 300",
  "position": "center",
  "children": [
    {
      "type": "vbox",
      "children": [
        {
          "type": "label",
          "text": "Hello, XGUI!"
        }
      ]
    }
  ]
}
```

## Error Handling

The tool provides helpful error messages for common issues:

- **File not found**: Checks if the JSON file exists before attempting to load
- **Invalid JSON**: Reports parsing errors from the XGUI JSON loader
- **Not a window**: Validates that the root object is a Window widget

## Features

- Command-line help with `--help` or no arguments
- Automatic window close event handling
- Support for all XGUI widgets and properties
- Dynamic class initialization for plugin support

## Tips

1. **Start Simple**: Begin with simple examples (01-05) before moving to complex ones
2. **Check Syntax**: Ensure your JSON is valid using a JSON validator
3. **Use Examples**: The `examples/` directory contains 18 ready-to-use examples
4. **Error Messages**: Read error messages carefully - they indicate what went wrong

## See Also

- `examples/` - Directory containing 18 example JSON files
- `examples/README.md` - Comprehensive guide to all examples
- `docs/` - Complete XGUI API documentation
- `output/gui.json` - Reference GUI definition

## Troubleshooting

### "Cannot open file"
- Verify the file path is correct
- Check file permissions
- Use absolute paths if relative paths aren't working

### "Failed to load JSON file"
- Validate JSON syntax (no trailing commas, proper quotes)
- Ensure all widget types are spelled correctly
- Check that required properties are present

### "Loaded object is not a Window"
- The root element must be of type "window"
- Check that "type": "window" is set at the top level

## Development

This tool is implemented in `tests/xgui-example/main.cpp` and demonstrates:

- Loading JSON files using `Master::LoadJson()`
- Dynamic class initialization with `Master::InitDynamicClasses()`
- Window event handling with `linkEvent()`
- Basic error checking and user feedback

It serves as both a useful utility and a code example for XGUI developers.

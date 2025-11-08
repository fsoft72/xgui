#!/usr/bin/env python3
"""
Simple GUI example for xgui Python3 bindings
Requires DISPLAY environment variable (X11 or Wayland)
"""

import sys
import os

# Add output directory to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../output'))

try:
    import xgui
except ImportError as e:
    print(f"Error: Could not import xgui module: {e}")
    print("Make sure you've built the Python bindings with 'make -f Makefile.gtk python'")
    sys.exit(1)

def on_button_click(widget):
    """Callback when button is clicked"""
    print("Button clicked!")
    return xgui.EVT_BLOCK

def on_window_close(widget):
    """Callback when window is closed"""
    print("Closing window...")
    xgui.Master.Quit()
    return xgui.EVT_BLOCK

def main():
    """Create a simple GUI window"""
    print("Creating xgui window...")

    # Create callbacks
    btn_callback = xgui.PyCallback(on_button_click)
    win_callback = xgui.PyCallback(on_window_close)

    # Create main window
    window = xgui.Master.CreateWindow()
    window.set("title", "XGUI Python3 Example")
    window.set("width", "400")
    window.set("height", "300")

    # Create vertical box container
    vbox = xgui.Master.CreateVBox(window, 10, True)

    # Create label
    label = xgui.Master.CreateLabel(vbox, "Hello from Python3!")
    label.set("font-size", "20")

    # Create button
    button = xgui.Master.CreateButton(vbox, "Click Me!", None, False)
    button.linkEvent("onclick", btn_callback)

    # Create entry field
    entry = xgui.Master.CreateEntry(vbox, "Type something here...", 0, True, False)

    # Make entry expand
    vbox.setExpand(label, False)
    vbox.setExpand(button, False)
    vbox.setExpand(entry, True)

    # Link window close event
    window.linkEvent("onclose", win_callback)

    # Show window and run event loop
    window.show()

    print("Running GUI event loop...")
    print("Close the window to exit.")

    xgui.Master.Run()

    print("GUI closed successfully!")

if __name__ == "__main__":
    # Check if we have a display
    if not os.environ.get('DISPLAY'):
        print("Warning: DISPLAY environment variable not set.")
        print("This example requires X11. If you're in a headless environment,")
        print("use 'xvfb-run python3 gui_example_python3.py' instead.")
        sys.exit(1)

    try:
        main()
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)

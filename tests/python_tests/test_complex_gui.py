#!/usr/bin/env python3
"""
Complex GUI Test Example for xgui

This test demonstrates:
- Creating a complex GUI programmatically
- Binding multiple event callbacks
- Interactive widget behaviors (sliders controlling progress bars, etc.)
- Cross-widget interactions

Run with: python3 test_complex_gui.py
Requires: DISPLAY environment variable (X11 or Wayland)

NOTE: This test creates the GUI programmatically. The complex_gui.json file
is provided as an example of GUI structure and can be used when LoadJson
is available in the Python bindings.
"""

import sys
import os

# Add the output directory to Python path to import xgui
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '../../output'))

try:
    import xgui
except ImportError as e:
    print(f"Error: Could not import xgui module: {e}")
    print("Make sure the xgui Python bindings are built in ../../output/")
    sys.exit(1)


class ComplexGUITest:
    """Test class managing the complex GUI and all its interactions"""

    def __init__(self):
        self.win = None
        self.widgets = {}
        self.callbacks = {}
        self.download_progress = 0

    def store_widget(self, name, widget):
        """Store a widget reference by name"""
        self.widgets[name] = widget
        widget.set("id", name)
        return widget

    def get_widget(self, widget_name):
        """Get a stored widget by name"""
        return self.widgets.get(widget_name)

    # ===== Volume Control Callbacks =====

    def on_volume_change(self, widget):
        """Callback when volume slider changes - updates progress bar and label"""
        try:
            value = int(widget.get("pos"))

            # Update progress bar
            progress = self.get_widget("volumeProgress")
            if progress:
                progress.set("pos", str(value))

            # Update label
            label = self.get_widget("volumeLabel")
            if label:
                label.set("text", f"{value}%")

            print(f"Volume changed to: {value}%")
        except Exception as e:
            print(f"Error in volume change callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Brightness Control Callbacks =====

    def on_brightness_slider_change(self, widget):
        """Callback when brightness slider changes - updates spin and label"""
        try:
            value = int(widget.get("pos"))

            # Update spin widget
            spin = self.get_widget("brightnessSpin")
            if spin:
                spin.set("pos", str(value))

            # Update label
            label = self.get_widget("brightnessLabel")
            if label:
                label.set("text", str(value))

            print(f"Brightness slider changed to: {value}")
        except Exception as e:
            print(f"Error in brightness slider callback: {e}")

        return xgui.EVT_BLOCK

    def on_brightness_spin_change(self, widget):
        """Callback when brightness spin changes - updates slider and label"""
        try:
            value = int(widget.get("pos"))

            # Update slider widget
            slider = self.get_widget("brightnessSlider")
            if slider:
                slider.set("pos", str(value))

            # Update label
            label = self.get_widget("brightnessLabel")
            if label:
                label.set("text", str(value))

            print(f"Brightness spin changed to: {value}")
        except Exception as e:
            print(f"Error in brightness spin callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Download Progress Callbacks =====

    def on_start_download(self, widget):
        """Callback when Start Download button is clicked"""
        try:
            print("Starting download simulation...")

            # Simulate incremental progress
            progress = self.get_widget("downloadProgress")
            label = self.get_widget("downloadLabel")
            status = self.get_widget("statusLabel")

            if self.download_progress < 100:
                self.download_progress += 10
                if self.download_progress > 100:
                    self.download_progress = 100

                if progress:
                    progress.set("pos", str(self.download_progress))

                if label:
                    label.set("text", f"Progress: {self.download_progress}%")

                if status:
                    if self.download_progress >= 100:
                        status.set("text", "Status: Download Complete!")
                    else:
                        status.set("text", f"Status: Downloading... {self.download_progress}%")

                print(f"Download progress: {self.download_progress}%")
            else:
                print("Download already complete!")

        except Exception as e:
            print(f"Error in start download callback: {e}")

        return xgui.EVT_BLOCK

    def on_reset_download(self, widget):
        """Callback when Reset button is clicked"""
        try:
            print("Resetting download...")

            self.download_progress = 0

            progress = self.get_widget("downloadProgress")
            label = self.get_widget("downloadLabel")
            status = self.get_widget("statusLabel")

            if progress:
                progress.set("pos", "0")

            if label:
                label.set("text", "Progress: 0%")

            if status:
                status.set("text", "Status: Ready")

        except Exception as e:
            print(f"Error in reset download callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Text Input Callbacks =====

    def on_text_entry_change(self, widget):
        """Callback when text entry changes - mirrors to label"""
        try:
            text = widget.get("text")

            # Update mirror label
            label = self.get_widget("mirrorLabel")
            if label:
                label.set("text", text)

            print(f"Text entry changed to: {text}")
        except Exception as e:
            print(f"Error in text entry callback: {e}")

        return xgui.EVT_BLOCK

    def on_counter_entry_change(self, widget):
        """Callback when counter entry changes - updates character count"""
        try:
            text = widget.get("text")
            char_count = len(text)

            # Update character count label
            label = self.get_widget("charCountLabel")
            if label:
                label.set("text", f"Characters: {char_count}")

            # Update progress bar (max 100 chars)
            progress = self.get_widget("charProgress")
            if progress:
                progress_value = min(char_count, 100)
                progress.set("pos", str(progress_value))

            print(f"Character count: {char_count}")
        except Exception as e:
            print(f"Error in counter entry callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Enable/Disable Callbacks =====

    def on_enable_checkbox_change(self, widget):
        """Callback when enable checkbox changes - enables/disables widgets"""
        try:
            checked = widget.get("checked") == "1"

            # Enable or disable advanced entry
            entry = self.get_widget("advancedEntry")
            if entry:
                entry.set("editable", "1" if checked else "0")

            print(f"Advanced settings {'enabled' if checked else 'disabled'}")
        except Exception as e:
            print(f"Error in enable checkbox callback: {e}")

        return xgui.EVT_BLOCK

    def on_apply_button_click(self, widget):
        """Callback when Apply Settings button is clicked"""
        try:
            checkbox = self.get_widget("enableCheckbox")
            entry = self.get_widget("advancedEntry")
            status = self.get_widget("statusLabel")

            if checkbox and checkbox.get("checked") == "1":
                text = entry.get("text") if entry else ""
                print(f"Applying settings: {text}")
                if status:
                    status.set("text", f"Status: Applied '{text}'")
            else:
                print("Advanced settings are disabled!")
                if status:
                    status.set("text", "Status: Enable advanced settings first")

        except Exception as e:
            print(f"Error in apply button callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Theme Callbacks =====

    def on_theme_change(self, widget):
        """Callback when theme combobox changes"""
        try:
            # Get selected index
            index = int(widget.get("pos"))
            themes = ["Light", "Dark", "Blue", "Green", "Red"]

            if 0 <= index < len(themes):
                theme = themes[index]

                # Update theme label
                label = self.get_widget("themeLabel")
                if label:
                    label.set("text", f"Selected: {theme}")

                print(f"Theme changed to: {theme}")
        except Exception as e:
            print(f"Error in theme change callback: {e}")

        return xgui.EVT_BLOCK

    # ===== Quit Callback =====

    def on_quit_click(self, widget):
        """Callback when Quit button is clicked"""
        print("Quit button clicked - exiting application")
        xgui.Master.Quit()
        return xgui.EVT_BLOCK

    def on_window_close(self, widget):
        """Callback when window is closed"""
        print("Window closed - exiting application")
        xgui.Master.Quit()
        return xgui.EVT_BLOCK

    # ===== Setup and Run =====

    def create_callback(self, func):
        """Helper to create and store a PyCallback"""
        callback = xgui.PyCallback(func)
        self.callbacks[func.__name__] = callback
        return callback

    def create_gui(self):
        """Create the GUI programmatically"""
        print("\nCreating GUI widgets...")

        # Create main window
        self.win = xgui.Master.CreateWindow()
        self.win.set("text", "Complex GUI Test - Interactive Widgets")
        self.win.set("size", "700 550")

        # Main vertical box
        main_vbox = xgui.Master.CreateVBox(self.win, 10)

        # Title label
        title = xgui.Master.CreateLabel(main_vbox, "Interactive Widget Demo")
        self.store_widget("titleLabel", title)

        # Create tab widget
        tab = xgui.Master.CreateTab(main_vbox, "top")
        self.store_widget("mainTab", tab)

        # === Tab 1: Sliders & Progress ===
        self.create_sliders_tab(tab)

        # === Tab 2: Input Controls ===
        self.create_input_tab(tab)

        # === Tab 3: Selection Controls ===
        self.create_selection_tab(tab)

        # Bottom status bar
        bottom_hbox = xgui.Master.CreateHBox(main_vbox, 10)
        status_label = xgui.Master.CreateLabel(bottom_hbox, "Status: Ready")
        self.store_widget("statusLabel", status_label)

        quit_button = xgui.Master.CreateButton(bottom_hbox, "Quit", None, False)
        self.store_widget("quitButton", quit_button)

        print("  ✓ GUI created successfully")

    def create_sliders_tab(self, tab):
        """Create the Sliders & Progress tab"""
        tab_vbox = xgui.Master.CreateVBox(tab, 15)
        tab_vbox.set("text", "Sliders & Progress")
        tab_vbox.set("expand", "1")

        # Volume Control Frame
        volume_frame = xgui.Master.CreateFrame(tab_vbox, "Volume Control")
        volume_vbox = xgui.Master.CreateVBox(volume_frame, 10)

        volume_hbox = xgui.Master.CreateHBox(volume_vbox, 10)
        xgui.Master.CreateLabel(volume_hbox, "Volume:")
        volume_label = xgui.Master.CreateLabel(volume_hbox, "50%")
        self.store_widget("volumeLabel", volume_label)

        volume_slider = xgui.Master.CreateSlider(volume_vbox, 0, 100, False)
        volume_slider.set("pos", "50")
        self.store_widget("volumeSlider", volume_slider)

        volume_progress = xgui.Master.CreateProgressbar(volume_vbox, 0, 100)
        volume_progress.set("pos", "50")
        self.store_widget("volumeProgress", volume_progress)

        # Brightness Control Frame
        brightness_frame = xgui.Master.CreateFrame(tab_vbox, "Brightness Control")
        brightness_vbox = xgui.Master.CreateVBox(brightness_frame, 10)

        brightness_hbox = xgui.Master.CreateHBox(brightness_vbox, 10)
        xgui.Master.CreateLabel(brightness_hbox, "Brightness:")
        brightness_label = xgui.Master.CreateLabel(brightness_hbox, "75")
        self.store_widget("brightnessLabel", brightness_label)

        brightness_slider = xgui.Master.CreateSlider(brightness_vbox, 0, 255, False)
        brightness_slider.set("pos", "75")
        self.store_widget("brightnessSlider", brightness_slider)

        brightness_spin = xgui.Master.CreateSpin(brightness_vbox, 0, 255)
        brightness_spin.set("pos", "75")
        self.store_widget("brightnessSpin", brightness_spin)

        # Download Progress Frame
        download_frame = xgui.Master.CreateFrame(tab_vbox, "Download Progress Simulator")
        download_vbox = xgui.Master.CreateVBox(download_frame, 10)

        download_label = xgui.Master.CreateLabel(download_vbox, "Progress: 0%")
        self.store_widget("downloadLabel", download_label)

        download_progress = xgui.Master.CreateProgressbar(download_vbox, 0, 100)
        download_progress.set("pos", "0")
        self.store_widget("downloadProgress", download_progress)

        download_hbox = xgui.Master.CreateHBox(download_vbox, 10)
        start_button = xgui.Master.CreateButton(download_hbox, "Start Download", None, False)
        self.store_widget("startButton", start_button)

        reset_button = xgui.Master.CreateButton(download_hbox, "Reset", None, False)
        self.store_widget("resetButton", reset_button)

    def create_input_tab(self, tab):
        """Create the Input Controls tab"""
        tab_vbox = xgui.Master.CreateVBox(tab, 15)
        tab_vbox.set("text", "Input Controls")
        tab_vbox.set("expand", "1")

        # Text Mirror Frame
        mirror_frame = xgui.Master.CreateFrame(tab_vbox, "Text Input Mirror")
        mirror_vbox = xgui.Master.CreateVBox(mirror_frame, 10)

        xgui.Master.CreateLabel(mirror_vbox, "Type something:")
        text_entry = xgui.Master.CreateEntry(mirror_vbox, "Hello World", 0, True, False)
        self.store_widget("textEntry", text_entry)

        xgui.Master.CreateLabel(mirror_vbox, "Mirror Output:")
        mirror_label = xgui.Master.CreateLabel(mirror_vbox, "Hello World")
        self.store_widget("mirrorLabel", mirror_label)

        # Character Counter Frame
        counter_frame = xgui.Master.CreateFrame(tab_vbox, "Character Counter")
        counter_vbox = xgui.Master.CreateVBox(counter_frame, 10)

        counter_entry = xgui.Master.CreateEntry(counter_vbox, "", 0, True, False)
        self.store_widget("counterEntry", counter_entry)

        char_count_label = xgui.Master.CreateLabel(counter_vbox, "Characters: 0")
        self.store_widget("charCountLabel", char_count_label)

        char_progress = xgui.Master.CreateProgressbar(counter_vbox, 0, 100)
        char_progress.set("pos", "0")
        self.store_widget("charProgress", char_progress)

        # Enable/Disable Frame
        enable_frame = xgui.Master.CreateFrame(tab_vbox, "Widget Enable/Disable")
        enable_vbox = xgui.Master.CreateVBox(enable_frame, 10)

        enable_checkbox = xgui.Master.CreateCheckbox(enable_vbox, "Enable Advanced Settings")
        enable_checkbox.set("checked", "0")
        self.store_widget("enableCheckbox", enable_checkbox)

        advanced_entry = xgui.Master.CreateEntry(enable_vbox, "Advanced Option", 0, False, False)
        self.store_widget("advancedEntry", advanced_entry)

        apply_button = xgui.Master.CreateButton(enable_vbox, "Apply Settings", None, False)
        self.store_widget("applyButton", apply_button)

    def create_selection_tab(self, tab):
        """Create the Selection Controls tab"""
        tab_vbox = xgui.Master.CreateVBox(tab, 15)
        tab_vbox.set("text", "Selection Controls")
        tab_vbox.set("expand", "1")

        # Theme Selector Frame
        theme_frame = xgui.Master.CreateFrame(tab_vbox, "Theme Selector")
        theme_vbox = xgui.Master.CreateVBox(theme_frame, 10)

        xgui.Master.CreateLabel(theme_vbox, "Select a theme:")
        theme_combo = xgui.Master.CreateCombobox(theme_vbox, False)
        theme_combo.set("text", "Light|Dark|Blue|Green|Red")
        theme_combo.set("pos", "0")
        self.store_widget("themeCombo", theme_combo)

        theme_label = xgui.Master.CreateLabel(theme_vbox, "Selected: Light")
        self.store_widget("themeLabel", theme_label)

    def bind_events(self):
        """Bind all event callbacks to widgets"""
        print("\nBinding events to widgets...")

        # Volume control
        volume_slider = self.get_widget("volumeSlider")
        if volume_slider:
            volume_slider.linkEvent("onchange", self.create_callback(self.on_volume_change))
            print("  ✓ Bound volume slider")

        # Brightness controls
        brightness_slider = self.get_widget("brightnessSlider")
        if brightness_slider:
            brightness_slider.linkEvent("onchange", self.create_callback(self.on_brightness_slider_change))
            print("  ✓ Bound brightness slider")

        brightness_spin = self.get_widget("brightnessSpin")
        if brightness_spin:
            brightness_spin.linkEvent("onchange", self.create_callback(self.on_brightness_spin_change))
            print("  ✓ Bound brightness spin")

        # Download buttons
        start_button = self.get_widget("startButton")
        if start_button:
            start_button.linkEvent("onclick", self.create_callback(self.on_start_download))
            print("  ✓ Bound start download button")

        reset_button = self.get_widget("resetButton")
        if reset_button:
            reset_button.linkEvent("onclick", self.create_callback(self.on_reset_download))
            print("  ✓ Bound reset button")

        # Text inputs
        text_entry = self.get_widget("textEntry")
        if text_entry:
            text_entry.linkEvent("onchange", self.create_callback(self.on_text_entry_change))
            print("  ✓ Bound text entry")

        counter_entry = self.get_widget("counterEntry")
        if counter_entry:
            counter_entry.linkEvent("onchange", self.create_callback(self.on_counter_entry_change))
            print("  ✓ Bound counter entry")

        # Enable checkbox and apply button
        enable_checkbox = self.get_widget("enableCheckbox")
        if enable_checkbox:
            enable_checkbox.linkEvent("onchange", self.create_callback(self.on_enable_checkbox_change))
            print("  ✓ Bound enable checkbox")

        apply_button = self.get_widget("applyButton")
        if apply_button:
            apply_button.linkEvent("onclick", self.create_callback(self.on_apply_button_click))
            print("  ✓ Bound apply button")

        # Theme selector
        theme_combo = self.get_widget("themeCombo")
        if theme_combo:
            theme_combo.linkEvent("onchange", self.create_callback(self.on_theme_change))
            print("  ✓ Bound theme combobox")

        # Quit button and window close
        quit_button = self.get_widget("quitButton")
        if quit_button:
            quit_button.linkEvent("onclick", self.create_callback(self.on_quit_click))
            print("  ✓ Bound quit button")

        if self.win:
            self.win.linkEvent("onclose", self.create_callback(self.on_window_close))
            print("  ✓ Bound window close event")

        print("\nAll events bound successfully!\n")

    def run(self):
        """Create the GUI and run the application"""
        print("=" * 60)
        print("Complex GUI Test - xgui Python Bindings")
        print("=" * 60)

        try:
            # Create the GUI
            self.create_gui()

            # Bind all events
            self.bind_events()

            # Show the window
            print("Showing window and starting event loop...")
            print("\nInteractive features:")
            print("  - Volume slider controls progress bar")
            print("  - Brightness slider and spin are synchronized")
            print("  - Download simulator with start/reset buttons")
            print("  - Text entry mirrors to label")
            print("  - Character counter with progress bar")
            print("  - Enable checkbox controls widget states")
            print("  - Theme selector updates display")
            print("\nClose the window or click Quit to exit.\n")

            self.win.set("visible", "1")

            # Run the event loop
            xgui.Master.Run()

            print("\nApplication exited successfully")
            return True

        except Exception as e:
            print(f"Error running application: {e}")
            import traceback
            traceback.print_exc()
            return False


def main():
    """Main entry point"""
    # Check if we have a display
    if "DISPLAY" not in os.environ:
        print("Warning: DISPLAY environment variable not set")
        print("This test requires a graphical environment (X11 or Wayland)")
        return 1

    # Create and run the test
    test = ComplexGUITest()
    success = test.run()

    return 0 if success else 1


if __name__ == "__main__":
    sys.exit(main())

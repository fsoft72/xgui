#!/usr/bin/env python3
"""
Complex GUI Test Example for xgui

This test demonstrates:
- Loading a complex GUI from JSON
- Binding multiple event callbacks
- Interactive widget behaviors (sliders controlling progress bars, etc.)
- Cross-widget interactions

Run with: python3 test_complex_gui.py
Requires: DISPLAY environment variable (X11 or Wayland)
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

    def get_widget(self, widget_id):
        """Get a widget by ID, caching it for future use"""
        if widget_id not in self.widgets:
            self.widgets[widget_id] = xgui.Master.GetWidgetById(widget_id)
        return self.widgets[widget_id]

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

    # ===== Selection Control Callbacks =====

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

    def on_service_checkbox_change(self, widget):
        """Callback when any service checkbox changes"""
        try:
            # Get all service checkboxes
            wifi = self.get_widget("wifiCheckbox")
            bluetooth = self.get_widget("bluetoothCheckbox")
            gps = self.get_widget("gpsCheckbox")

            # Build list of active services
            services = []
            if wifi and wifi.get("checked") == "1":
                services.append("WiFi")
            if bluetooth and bluetooth.get("checked") == "1":
                services.append("Bluetooth")
            if gps and gps.get("checked") == "1":
                services.append("GPS")

            # Update services label
            label = self.get_widget("servicesLabel")
            if label:
                if services:
                    label.set("text", f"Active Services: {', '.join(services)}")
                else:
                    label.set("text", "Active Services: None")

            print(f"Active services: {services}")
        except Exception as e:
            print(f"Error in service checkbox callback: {e}")

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

        # Service checkboxes
        wifi_checkbox = self.get_widget("wifiCheckbox")
        if wifi_checkbox:
            wifi_checkbox.linkEvent("onchange", self.create_callback(self.on_service_checkbox_change))
            print("  ✓ Bound WiFi checkbox")

        bluetooth_checkbox = self.get_widget("bluetoothCheckbox")
        if bluetooth_checkbox:
            bluetooth_checkbox.linkEvent("onchange", self.create_callback(self.on_service_checkbox_change))
            print("  ✓ Bound Bluetooth checkbox")

        gps_checkbox = self.get_widget("gpsCheckbox")
        if gps_checkbox:
            gps_checkbox.linkEvent("onchange", self.create_callback(self.on_service_checkbox_change))
            print("  ✓ Bound GPS checkbox")

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
        """Load the GUI from JSON and run the application"""
        print("=" * 60)
        print("Complex GUI Test - xgui Python Bindings")
        print("=" * 60)

        # Get the JSON file path
        json_path = os.path.join(os.path.dirname(__file__), "complex_gui.json")

        if not os.path.exists(json_path):
            print(f"Error: JSON file not found at {json_path}")
            return False

        print(f"\nLoading GUI from: {json_path}")

        try:
            # Load the GUI from JSON
            self.win = xgui.Master.LoadJson(json_path)

            if not self.win:
                print("Error: Failed to load GUI from JSON")
                return False

            print("✓ GUI loaded successfully")

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
            print("  - Service checkboxes update status")
            print("\nClose the window or click Quit to exit.\n")

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

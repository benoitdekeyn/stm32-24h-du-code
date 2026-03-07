#!/usr/bin/env python3
"""
Kiosk Browser for STM32MP157 OpenSTLinux
Displays the IoT LED Controller interface in fullscreen using GTK4 + WebKit6
"""

import sys
import gi

gi.require_version("Gtk", "4.0")
gi.require_version("WebKit", "6.0")
from gi.repository import Gtk, WebKit


def main():
    """Launch fullscreen kiosk browser"""
    url = sys.argv[1] if len(sys.argv) > 1 else "http://127.0.0.1:5000"
    
    print(f"Launching kiosk browser: {url}")
    
    app = Gtk.Application(application_id="com.stm32.iot.ledcontroller")

    def on_activate(application):
        """Create and display fullscreen window with WebView"""
        window = Gtk.ApplicationWindow(application=application)
        window.set_title("IoT LED Controller")
        window.fullscreen()

        webview = WebKit.WebView()
        webview.load_uri(url)
        window.set_child(webview)
        window.present()

    app.connect("activate", on_activate)
    app.run(None)


if __name__ == "__main__":
    main()

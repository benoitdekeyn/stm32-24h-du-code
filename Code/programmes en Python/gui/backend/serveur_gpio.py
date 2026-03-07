#!/usr/bin/env python3
"""
Flask API Server with real GPIO control for STM32MP157
Controls the orange LED (PH7) via web interface
"""

from flask import Flask, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__, static_folder='./frontend/dist')

# LED state tracking
led_state = False

# GPIO configuration (from led_bouton.py)
LED_CHIP = "gpiochip4"
LED_PIN = "11"  # Vert D11


def set_led(state):
    """Control the physical LED via GPIO"""
    try:
        # Kill any existing gpioset process to avoid conflicts
        subprocess.run(["killall", "gpioset"], stderr=subprocess.DEVNULL)
        
        # Set LED state (1=ON, 0=OFF)
        value = "1" if state else "0"
        subprocess.run(
            ["gpioset", "-c", LED_CHIP, "-z", f"{LED_PIN}={value}"],
            check=True,
            stderr=subprocess.PIPE
        )
        return True
    except subprocess.CalledProcessError as e:
        print(f"GPIO Error: {e}")
        return False
    except FileNotFoundError:
        print("GPIO tools not available (gpioset not found)")
        return False


@app.route('/')
def index():
    """Serve the main page"""
    return send_from_directory(app.static_folder, 'index.html')


@app.route('/api/toggle', methods=['POST'])
def toggle_led():
    """Toggle the LED state and control physical GPIO"""
    global led_state
    
    # Toggle state
    led_state = not led_state
    
    # Control physical LED
    success = set_led(led_state)
    
    status_text = "ON" if led_state else "OFF"
    message = f"LED is {status_text}"
    print(f"LED toggled: {status_text} (GPIO: {'OK' if success else 'FAILED'})")
    
    return jsonify({
        'status': 'success',
        'message': message,
        'gpio_success': success
    })


@app.route('/<path:path>')
def static_files(path):
    """Serve static files (CSS, JS)"""
    return send_from_directory(app.static_folder, path)


if __name__ == '__main__':
    print("=" * 60)
    print("  IoT LED Controller with GPIO Control")
    print("=" * 60)
    print(f"  LED: {LED_CHIP} pin {LED_PIN}")
    print(f"  Web UI: http://0.0.0.0:5000")
    print("=" * 60)
    
    # Initialize LED to OFF state
    set_led(False)
    
    # Start Flask server
    app.run(host='0.0.0.0', port=5000, debug=False)

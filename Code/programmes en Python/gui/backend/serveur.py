from flask import Flask, jsonify, send_from_directory
import os

app = Flask(__name__, static_folder='./frontend/dist')

# Simulate LED state
led_state = False

@app.route('/')
def serve_frontend():
    """Serve the main index.html from the Vite build output"""
    return send_from_directory(app.static_folder, 'index.html')

@app.route('/<path:path>')
def serve_static(path):
    """Serve static files (CSS, JS, etc.) from the dist directory"""
    try:
        return send_from_directory(app.static_folder, path)
    except:
        # If file not found, serve index.html for client-side routing
        return send_from_directory(app.static_folder, 'index.html')

@app.route('/api/toggle', methods=['POST'])
def toggle_led():
    """Toggle the LED and return the new state"""
    global led_state
    
    # Toggle the LED state
    led_state = not led_state
    
    # In a real application, you would control the actual GPIO pin here
    # For now, just print to console for simulation
    status_message = "LED is ON" if led_state else "LED is OFF"
    print(f"[LED] {status_message}")
    
    # Return JSON response
    return jsonify({
        "status": "success",
        "message": status_message
    })

if __name__ == '__main__':
    print("Starting IoT LED Server...")
    print("Server will be accessible at http://0.0.0.0:5000")
    app.run(host='0.0.0.0', port=5000, debug=True)

#!/bin/bash
#
# Startup script for IoT LED Controller on STM32MP157
# This script starts the Flask server and launches the kiosk browser
#

# Configuration
APP_DIR="/root/serveur_led"
DISPLAY_URL="http://127.0.0.1:5000"

# Set up environment for Wayland/Weston (autodetected)
export GDK_BACKEND=wayland

detect_wayland_env() {
    # Prefer an existing Wayland socket created by the graphical session.
    SOCKET_PATH=$(find /run/user -maxdepth 2 -type s -name "wayland-*" 2>/dev/null | head -n 1)
    if [ -n "$SOCKET_PATH" ]; then
        export XDG_RUNTIME_DIR=$(dirname "$SOCKET_PATH")
        export WAYLAND_DISPLAY=$(basename "$SOCKET_PATH")
    else
        # Fallback if no socket is found yet.
        export XDG_RUNTIME_DIR=/run/user/0
        mkdir -p "$XDG_RUNTIME_DIR"
        export WAYLAND_DISPLAY=wayland-0
    fi
    echo "Wayland runtime: $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY"
}

echo "========================================="
echo "  IoT LED Controller - Startup"
echo "========================================="

# Function to start Flask server
start_server() {
    echo "[1/3] Starting Flask server with GPIO control..."
    cd $APP_DIR
    
    # Kill any existing server
    pkill -f "python3 serveur"
    sleep 1
    
    # Start server in background
    nohup python3 serveur_gpio.py > serveur.log 2>&1 &
    SERVER_PID=$!
    echo "Flask server started (PID: $SERVER_PID)"
    
    # Wait for server to be ready
    echo "Waiting for server to start..."
    for i in {1..10}; do
        if curl -s http://127.0.0.1:5000/ > /dev/null 2>&1; then
            echo "Server is ready!"
            return 0
        fi
        sleep 1
    done
    echo "Warning: Server may not be ready yet"
}

# Function to start Weston (if not running)
start_weston() {
    if ! pgrep -x "weston" > /dev/null; then
        echo "[2/3] Starting Weston compositor..."
        # Start Weston in background
        weston --backend=drm-backend.so > /tmp/weston.log 2>&1 &
        sleep 3
        detect_wayland_env
        echo "Weston started"
    else
        echo "[2/3] Weston already running"
        detect_wayland_env
    fi
}

# Function to start kiosk browser
start_kiosk() {
    echo "[3/3] Launching kiosk browser..."
    cd $APP_DIR

    # Ensure only one kiosk process is running
    pkill -f "kiosk_browser.py" || true
    sleep 1

    # Run kiosk browser as graphical session user for Wayland compatibility.
    cp "$APP_DIR/kiosk_browser.py" /tmp/kiosk_browser.py
    chmod 755 /tmp/kiosk_browser.py
    su -s /bin/sh weston -c "XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR WAYLAND_DISPLAY=$WAYLAND_DISPLAY GDK_BACKEND=wayland LIBGL_ALWAYS_SOFTWARE=1 GSK_RENDERER=cairo WEBKIT_DISABLE_COMPOSITING_MODE=1 python3 -u /tmp/kiosk_browser.py $DISPLAY_URL" 2>&1 | tee kiosk.log &
    KIOSK_PID=$!
    echo "Kiosk browser started (PID: $KIOSK_PID)"
}

# Main execution
start_server
start_weston
start_kiosk

echo "========================================="
echo "  IoT LED Controller is running!"
echo "========================================="
echo ""
echo "Server Log: $APP_DIR/serveur.log"
echo "Kiosk Log:  $APP_DIR/kiosk.log"
echo ""
echo "To stop:"
echo "  pkill -f serveur.py"
echo "  pkill -f kiosk_browser.py"
echo ""

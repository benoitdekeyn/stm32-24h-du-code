#!/bin/bash
#
# Install Kiosk Browser dependencies on STM32MP157 OpenSTLinux
# Required packages: python3-pygobject, webkitgtk
#

echo "========================================="
echo "  Installing Kiosk Dependencies"
echo "========================================="
echo ""

# Update package lists
echo "Updating package lists..."
apt-get update

# Install GTK4 + WebKit6 dependencies for Python
echo ""
echo "Installing python3-pygobject and webkitgtk..."
apt-get install -y python3-pygobject webkitgtk

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================="
    echo "  Installation Successful!"
    echo "========================================="
    echo ""
    echo "Installed packages:"
    echo "  - python3-pygobject (3.46.0)"
    echo "  - webkitgtk (2.44.4)"
    echo ""
    echo "You can now run the kiosk browser:"
    echo "  ./start_kiosk.sh"
    echo ""
else
    echo ""
    echo "========================================="
    echo "  Installation Failed!"
    echo "========================================="
    echo ""
    echo "Alternative: Access the app remotely at http://192.168.7.1:5000"
    echo ""
    exit 1
fi

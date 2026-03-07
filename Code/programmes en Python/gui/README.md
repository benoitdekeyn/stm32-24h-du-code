# IoT LED Controller

A hybrid web application for controlling LEDs on an STM32MP157 embedded Linux board.

## Architecture

- **Frontend**: Vite + Vanilla JavaScript
- **Backend**: Python Flask API with GPIO control
- **Deployment**: PowerShell script for SSH/SCP deployment
- **Hardware**: STM32MP157 with physical LED control (PH7 - Orange LED)

## Project Structure

```
gui/
├── frontend/               # Vite + Vanilla JS frontend
│   ├── package.json       # npm dependencies and scripts
│   ├── index.html         # Main HTML file
│   ├── style.css          # Modern UI styling
│   └── main.js            # LED toggle logic with fetch API
├── backend/               # Flask backend
│   ├── serveur.py         # Flask server (demo mode)
│   ├── serveur_gpio.py    # Flask server with real GPIO control ⚡
│   ├── kiosk_browser.py   # Fullscreen browser for STM32 display
│   ├── start_kiosk.sh     # Startup script
│   └── install_kiosk_deps.sh  # Dependency installer
└── deploy.ps1             # PowerShell deployment script
```

**Note:** `serveur_gpio.py` controls the physical orange LED (PH7) on the STM32MP157 board using GPIO commands.

## Setup Instructions

### 1. Install Frontend Dependencies

Navigate to the frontend directory and install npm packages:

```powershell
cd gui/frontend
npm install
```

### 2. Development Mode

To run the frontend in development mode with hot reload:

```powershell
npm run dev
```

The development server will start at `http://localhost:5173`

### 3. Build Frontend

To build the frontend for production:

```powershell
npm run build
```

This creates an optimized build in `frontend/dist/`

### 4. Install Backend Dependencies

Make sure you have Flask installed:

```powershell
pip install flask
```

### 5. Test Locally

Before deploying, you can test the application locally:

```powershell
# From gui/frontend directory
npm run build

# From gui/backend directory
cd ../backend
python serveur.py
```

Access the application at `http://localhost:5000`

### 6. Deploy to STM32MP157 Board

**Prerequisites:**
- SSH access to the board configured
- Password-less SSH recommended (use SSH keys)
- Python 3 and Flask installed on the board

**Deploy command:**

```powershell
# From gui/frontend directory
npm run deploy
```

Or manually run the deployment script:

```powershell
# From gui directory
cd ..
.\deploy.ps1
```

**Deployment Configuration:**

Edit `deploy.ps1` to change:
- `$TargetIP = "192.168.7.1"` - Your board's IP address
- `$TargetUser = "root"` - SSH user
- `$DestDir = "/root/serveur_led"` - Destination directory on board

## API Endpoints

- `GET /` - Serve the frontend application
- `POST /api/toggle` - Toggle LED state and control physical GPIO

**Response format:**
```json
{
  "status": "ON",  // or "OFF"
  "gpio_success": true  // GPIO control success status
}
```

## GPIO Control

The application controls the **orange LED (PH7)** on the STM32MP157 Discovery board.

**Hardware configuration:**
- GPIO Chip: `gpiochip7`
- LED Pin: `7`
- Commands: `gpioset -c gpiochip7 -z 7=1` (ON) / `7=0` (OFF)

The `serveur_gpio.py` backend executes real GPIO commands when you click the toggle button in the web interface.

## Features

- ✨ Modern, responsive UI with gradient design
- 🎯 Large, easy-to-use toggle button
- 📊 Real-time LED status display
- ⚡ **Real GPIO control** of physical LED on STM32MP157
- 🖥️ Fullscreen kiosk mode on STM32 display
- 🚀 One-command deployment to embedded board
- 🔄 Automatic server restart on deployment

## Development Notes

- The frontend uses Vite for fast development and optimized builds
- Pure Vanilla JavaScript - no frameworks required
- Flask serves both the API and static files
- PowerShell script handles automated deployment over SSH
- **serveur_gpio.py** uses `subprocess` to execute GPIO commands (`gpioset`)
- Kiosk mode uses GTK4 + WebKit6 for fullscreen display

## Testing

### On PC (development - no GPIO)
```powershell
cd gui/backend
python serveur.py  # Demo server without GPIO control
```
Access at `http://localhost:5000`

### On STM32 (production - real GPIO)
```bash
ssh root@192.168.7.1
cd /root/serveur_led
python3 serveur_gpio.py  # GPIO control enabled
```

**Verify GPIO works:**
1. Access web UI at `http://192.168.7.1:5000`
2. Click the toggle button
3. Watch the orange LED on the board turn on/off physically
4. Check logs: `tail -f serveur.log` should show "LED toggled: ON (GPIO: OK)"

## Troubleshooting

### "Cannot find module" errors
Run `npm install` in the frontend directory

### SSH connection errors
- Verify board IP address in `deploy.ps1`
- Ensure SSH is enabled on the board
- Test SSH connection: `ssh root@192.168.7.1`

### Flask not found
Install Flask: `pip install flask`

### Port 5000 already in use
Change the port in `backend/serveur_gpio.py` (line with `app.run`)

### GPIO control not working
- Verify GPIO tools are installed: `which gpioset`
- Test manually: `gpioset -c gpiochip7 -z 7=1` (should light LED)
- Check server logs for GPIO errors: `tail -f /root/serveur_led/serveur.log`

### LED doesn't respond to web button
- Make sure `serveur_gpio.py` is running (not `serveur.py`)
- Check process: `ps aux | grep serveur_gpio`
- Verify deployment copied the correct file

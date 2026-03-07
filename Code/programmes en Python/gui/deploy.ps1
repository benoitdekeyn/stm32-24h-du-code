# ============================================
# IoT LED Application Deployment Script
# ============================================
# This script deploys the Flask backend and Vite frontend to an embedded Linux board

# Configuration
$TargetIP = "192.168.7.1"
$TargetUser = "root"
$DestDir = "/root/serveur_led"

# Get the directory where this script is located (gui directory)
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BackendDir = Join-Path $ScriptDir "backend"
$FrontendDistDir = Join-Path $ScriptDir "frontend\dist"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  IoT LED Application Deployment" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Target: $TargetUser@$TargetIP" -ForegroundColor Yellow
Write-Host "Destination: $DestDir" -ForegroundColor Yellow
Write-Host ""

# Step 1: Ensure remote directory exists
Write-Host "[1/5] Creating remote directory..." -ForegroundColor Green
ssh "$TargetUser@$TargetIP" "mkdir -p $DestDir/frontend"

if ($LASTEXITCODE -ne 0) {
    Write-Host "Failed to create remote directory. Check SSH connection." -ForegroundColor Red
    exit 1
}

# Step 2: Copy backend files (server, kiosk browser, startup script)
Write-Host "[2/5] Deploying backend files..." -ForegroundColor Green
scp (Join-Path $BackendDir "serveur.py") "$TargetUser@${TargetIP}:$DestDir/"
scp (Join-Path $BackendDir "serveur_gpio.py") "$TargetUser@${TargetIP}:$DestDir/"
scp (Join-Path $BackendDir "kiosk_browser.py") "$TargetUser@${TargetIP}:$DestDir/"
scp (Join-Path $BackendDir "start_kiosk.sh") "$TargetUser@${TargetIP}:$DestDir/"
scp (Join-Path $BackendDir "install_kiosk_deps.sh") "$TargetUser@${TargetIP}:$DestDir/"

if ($LASTEXITCODE -ne 0) {
    Write-Host "Failed to copy backend files." -ForegroundColor Red
    exit 1
}

# Make scripts executable
ssh "$TargetUser@$TargetIP" "chmod +x $DestDir/start_kiosk.sh $DestDir/install_kiosk_deps.sh"

# Step 3: Copy frontend dist directory
Write-Host "[3/5] Deploying frontend (dist folder)..." -ForegroundColor Green
scp -r $FrontendDistDir "$TargetUser@${TargetIP}:$DestDir/frontend/"

if ($LASTEXITCODE -ne 0) {
    Write-Host "Failed to copy frontend files." -ForegroundColor Red
    exit 1
}

# Step 4: Stop existing processes
Write-Host "[4/5] Stopping existing processes..." -ForoundColor Green
ssh "$TargetUser@$TargetIP" "pkill -f 'python3 serveur.py' || true; pkill -f 'kiosk_browser.py' || true"

# Step 5: Start the application (server + kiosk browser)
Write-Host "[5/5] Starting application on remote board..." -ForegroundColor Green
ssh "$TargetUser@$TargetIP" "cd $DestDir && nohup ./start_kiosk.sh > startup.log 2>&1 &"

if ($LASTEXITCODE -ne 0) {
    Write-Host "Failed to start application." -ForegroundColor Red
    exit 1
}

# Give it a moment to start
Start-Sleep -Seconds 2

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Deployment Successful!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Server Status:" -ForegroundColor Green
Write-Host "  Remote access: http://$TargetIP:5000" -ForegroundColor White
Write-Host ""
Write-Host "Display on STM32 screen:" -ForegroundColor Yellow
Write-Host "  Run this command on the STM32:" -ForegroundColor White
Write-Host "    ssh $TargetUser@$TargetIP 'cd $DestDir && ./install_kiosk_deps.sh'" -ForegroundColor Gray
Write-Host "  Then start kiosk mode:" -ForegroundColor White
Write-Host "    ssh $TargetUser@$TargetIP 'cd $DestDir && ./start_kiosk.sh'" -ForegroundColor Gray
Write-Host ""
Write-Host "  See KIOSK_SETUP.md for detailed instructions" -ForegroundColor Cyan
Write-Host ""
Write-Host "Logs:" -ForegroundColor Yellow
Write-Host "  Server:  ssh $TargetUser@$TargetIP 'tail -f $DestDir/serveur.log'" -ForegroundColor Gray
Write-Host "  Kiosk:   ssh $TargetUser@$TargetIP 'tail -f $DestDir/kiosk.log'" -ForegroundColor Gray
Write-Host ""

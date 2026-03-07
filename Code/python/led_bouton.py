#!/usr/bin/env python3
import subprocess

while True:
    # Lire l'état du bouton USER2 (PA13)
    button_pressed = "inactive" in subprocess.getoutput("gpioget -c gpiochip0 13")
    
    # Contrôler la LED orange (PH7)
    if button_pressed:
        subprocess.run(["killall", "gpioset"], stderr=subprocess.DEVNULL)
        subprocess.run(["gpioset", "-c", "gpiochip7", "-z", "7=1"])
    else:
        subprocess.run(["killall", "gpioset"], stderr=subprocess.DEVNULL)
        subprocess.run(["gpioset", "-c", "gpiochip7", "-z", "7=0"])
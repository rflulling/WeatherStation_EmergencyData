# Quick Start Guide

This guide will help you get your ESP32 Weather Station up and running in minutes.

## Prerequisites

Before you begin, make sure you have:
- [ ] ESP32 development board
- [ ] BME280 sensor
- [ ] SCD30 CO2 sensor
- [ ] Breadboard and jumper wires
- [ ] USB cable for ESP32
- [ ] Computer with Visual Studio Code installed

## Step-by-Step Setup

### 1. Install Software (First Time Only)

1. **Download and install Visual Studio Code**
   - Visit: https://code.visualstudio.com/
   - Download for your operating system
   - Install and launch VS Code

2. **Install PlatformIO Extension**
   - Open VS Code
   - Click the Extensions icon (or press `Ctrl+Shift+X`)
   - Search for "PlatformIO IDE"
   - Click "Install" on the official PlatformIO IDE extension
   - Wait for installation to complete (may take a few minutes)
   - Restart VS Code if prompted

### 2. Hardware Setup

1. **Wire the sensors** according to this diagram:

```
ESP32          BME280/SCD30
-----          ------------
3.3V    -->    VCC
GND     -->    GND
GPIO21  -->    SDA
GPIO22  -->    SCL
```

2. **Connect ESP32 to computer** via USB cable

See `HARDWARE.md` for detailed wiring diagrams and troubleshooting.

### 3. Open the Project

1. **Clone or download this repository**
   ```bash
   git clone https://github.com/rflulling/WeatherStation_EmergencyData.git
   ```

2. **Open in VS Code**
   - Launch VS Code
   - Click "File" → "Open Folder"
   - Navigate to the `WeatherStation_EmergencyData` folder
   - Click "Select Folder"

3. **Wait for PlatformIO to initialize**
   - PlatformIO will automatically detect the project
   - Wait for "PlatformIO: Home" to appear in the bottom status bar

### 4. Build and Upload

1. **Build the project**
   - Click the checkmark (✓) icon in the bottom status bar
   - Or press `Ctrl+Alt+B`
   - Wait for build to complete (first build downloads libraries, may take 5-10 minutes)

2. **Upload to ESP32**
   - Click the right arrow (→) icon in the bottom status bar
   - Or press `Ctrl+Alt+U`
   - Wait for upload to complete

3. **Open Serial Monitor**
   - Click the plug icon in the bottom status bar
   - Or press `Ctrl+Alt+S`
   - You should see sensor readings every 5 seconds!

## What You Should See

Once running, the Serial Monitor will display:

```
========================================
    WEATHER STATION - EMERGENCY DATA
========================================

--- Environmental Conditions ---
Temperature: 23.5 °C
Humidity: 45.2 %
Pressure: 1013.2 hPa
Altitude: 100.5 m
Status: Normal Pressure - Stable Conditions

--- Air Quality ---
CO2: 450 ppm
Status: CO2 Normal
========================================
```

## Troubleshooting

### "No sensors detected!"

**Problem**: Serial Monitor shows "No sensors detected!"

**Solutions**:
1. Check all wiring connections
2. Verify sensors are powered (LED on sensor boards should be lit)
3. Try one sensor at a time to isolate the issue
4. Use an I2C scanner sketch to verify addresses

### "Upload failed" or "Device not found"

**Problem**: Cannot upload code to ESP32

**Solutions**:
1. Check USB cable (must be data cable, not charging-only)
2. Install CH340/CP2102 USB drivers if needed
3. Try a different USB port
4. Hold the BOOT button during upload
5. Check Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac) for port

### Build errors

**Problem**: Build fails with library errors

**Solutions**:
1. Delete `.pio` folder in project directory
2. Click PlatformIO icon → Project Tasks → Clean
3. Rebuild the project
4. Check internet connection (needed to download libraries)

### Incorrect readings

**Problem**: Sensor readings seem wrong

**Solutions**:
1. Give sensors 5-10 minutes to stabilize after power-on
2. Ensure sensors are not near heat sources
3. For altitude: Adjust `SEALEVEL_PRESSURE_HPA` in code to your local sea level pressure
4. For CO2: SCD30 needs several hours to calibrate initially

## Next Steps

- **Data Logging**: Add SD card module to log data over time
- **Display**: Add OLED display for standalone operation  
- **WiFi**: Enable remote monitoring via web interface
- **Alerts**: Add buzzer or LED for threshold warnings
- **Power**: Add battery for portable operation

## Getting Help

- Check `README.md` for detailed documentation
- Review `HARDWARE.md` for wiring help
- Check PlatformIO forums: https://community.platformio.org/
- Open an issue on GitHub if you find bugs

## Safety Notes

⚠️ **Important Safety Information**:

1. **CO2 Levels**:
   - < 1000 ppm: Normal, safe air
   - 1000-2000 ppm: Ventilate the area
   - \> 2000 ppm: Dangerous, evacuate immediately

2. **Pressure Changes**:
   - Rapid pressure drops may indicate severe weather
   - Track trends over hours/days for best results

3. **Emergency Use**:
   - This device is for informational purposes
   - Do not rely solely on this device for life-safety decisions
   - Always follow official emergency guidance

Enjoy your weather station! 🌤️📊

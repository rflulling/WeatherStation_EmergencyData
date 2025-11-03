# Quick Start Guide

## Getting Started in 5 Minutes

### 1. Hardware Setup

**You'll need:**
- Arduino board (Uno, Mega, ESP32, etc.)
- BMP280 or BME280 pressure sensor
- Jumper wires

**Connections:**
```
Sensor  →  Arduino
─────────────────
VCC     →  3.3V or 5V
GND     →  GND
SDA     →  SDA (A4 on Uno)
SCL     →  SCL (A5 on Uno)
```

### 2. Install Libraries

In Arduino IDE:
1. Go to **Sketch** → **Include Library** → **Manage Libraries**
2. Search and install: `Adafruit BMP280`
3. (This will also install required dependencies)

### 3. Load the Code

Option A: **Use the main sketch**
```
Open: WeatherStation_EmergencyData.ino
```

Option B: **Use the example**
```
Open: examples/PressureMonitoring/PressureMonitoring.ino
```

### 4. Configure for Your Sensor

Uncomment the sensor initialization code:

```cpp
// Add at the top
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;

// In setup()
if (!bmp.begin()) {
  Serial.println("Could not find sensor!");
  while (1) delay(10);
}

// In readPressureSensor() function
return bmp.readPressure() / 100.0F; // Convert Pa to hPa
```

### 5. Upload and Run

1. Connect Arduino via USB
2. Select board and port in Arduino IDE
3. Click **Upload** (→)
4. Open **Serial Monitor** (9600 baud)
5. Watch the data!

## What You'll See

### Initial Phase (First 20 samples)
```
Sample #1 | Raw: 1013.25 hPa | Smoothed: 1013.25 hPa | Baseline in 19 samples
Sample #2 | Raw: 1013.30 hPa | Smoothed: 1013.28 hPa | Baseline in 18 samples
...
```

### Normal Operation
```
Sample #25 | Raw: 1013.45 hPa | Smoothed: 1013.40 hPa | Baseline: 1013.35 hPa | Δ: +0.05 hPa
```

### Alert Example
```
╔════════════════════════════════════════╗
║   ⚠️  SEVERE WEATHER ALERT  ⚠️         ║
╚════════════════════════════════════════╝
Pressure change: -3.25 hPa

⬇️  RAPID PRESSURE DROP DETECTED
Indication: Storm system approaching
Recommendation: Seek shelter immediately
Status: Severe weather likely
════════════════════════════════════════
```

## Configuration Options

### Adjust Sample Window
```cpp
const int SAMPLE_SIZE = 20;  // Change to 10, 30, etc.
```

### Adjust Alert Threshold
```cpp
const float PRESSURE_THRESHOLD = 3.0;  // Change to 2.0, 5.0, etc.
```

### Adjust Sample Rate
```cpp
const unsigned long SAMPLE_INTERVAL = 60000;  // 60 seconds
// 30000 = 30 seconds
// 300000 = 5 minutes
```

## Testing Without Sensor

You can test the code without a physical sensor:

1. The default code includes a simulator
2. It generates realistic pressure variations
3. After ~50 samples, it simulates a storm
4. You'll see alerts trigger

To test with Python simulation:
```bash
python3 simulate_moving_average.py
```

## Understanding the Output

| Column | Meaning |
|--------|---------|
| Sample # | Number of readings taken |
| Raw | Direct sensor reading (hPa) |
| Smoothed | Moving average value (hPa) |
| Baseline | Established normal pressure (hPa) |
| Δ | Change from baseline (hPa) |
| Alert | Severe weather warning |

## Pressure Change Interpretation

| Change | Meaning | Action |
|--------|---------|--------|
| 0 to ±1 hPa | Normal variation | No action needed |
| ±1 to ±2 hPa | Minor change | Monitor conditions |
| ±2 to ±3 hPa | Significant change | Prepare for weather change |
| **> ±3 hPa** | **Severe change** | **Alert triggered** |
| < -3 hPa | Storm approaching | Seek shelter |
| > +3 hPa | Weather improving | Conditions stabilizing |

## Troubleshooting

### No Serial Output
- Check baud rate is 9600
- Check USB connection
- Verify correct port selected

### Sensor Not Found
- Check wiring
- Verify I2C address (0x76 or 0x77)
- Try: `bmp.begin(0x76)` or `bmp.begin(0x77)`

### Crazy Readings
- Check sensor is not obstructed
- Verify stable power supply
- Ensure good connections

### No Alerts
- Check threshold setting
- Verify sensor is working
- May need to wait for natural pressure changes
- Or test with simulation code

### Constant Alerts
- Threshold may be too sensitive
- Increase `PRESSURE_THRESHOLD` value
- Check for sensor issues

## Next Steps

### Enhance Your System

1. **Add Display**
   - LCD screen to show current readings
   - LED indicators for alerts

2. **Add Data Logging**
   - SD card for historical data
   - Graph pressure trends

3. **Add Notifications**
   - Buzzer for audio alerts
   - Wi-Fi/Bluetooth alerts to phone

4. **Multiple Sensors**
   - Temperature
   - Humidity  
   - Wind speed

### Example: Add LCD Display
```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void displayOnLCD(float pressure, float change) {
  lcd.clear();
  lcd.print("P: ");
  lcd.print(pressure, 1);
  lcd.setCursor(0, 1);
  lcd.print("Delta: ");
  lcd.print(change, 1);
}
```

## Resources

- **Full Documentation**: See README.md
- **Algorithm Details**: See ALGORITHM.md
- **Visual Examples**: See VISUALIZATION.md
- **Test Code**: test_moving_average.ino
- **Example Code**: examples/PressureMonitoring/

## Support

For issues or questions:
1. Check this documentation
2. Review the example code
3. Run the test simulation
4. Check GitHub issues

Happy monitoring! 🌤️ ⛈️ 🌈

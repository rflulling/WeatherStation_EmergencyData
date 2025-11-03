# WeatherStation_EmergencyData

It's easy enough to get weather data online, and to know when it's coming straight to you. But what about when it's on top of you? When is it safe to come out? This project aims to collect data useful in identifying when weather has normalized enough to leave shelter and return home.

## Overview

This weather station monitors atmospheric pressure using a **20-sample moving average** to smooth out spikes in the data. The system establishes a baseline for typical pressure in the installation location, then applies rules to detect severe weather based on significant pressure shifts.

## Features

- **Moving Average Filter**: 20-sample rolling window that smooths pressure data by continuously replacing the oldest sample with the newest
- **Baseline Establishment**: Automatically establishes typical operational pressure for the installation location
- **Severe Weather Detection**: Alerts when pressure changes exceed configurable thresholds
- **Real-time Monitoring**: Continuous pressure tracking with customizable sampling intervals
- **Room-relative Readings**: Measures pressure relative to the installation environment

## How It Works

### Moving Average (Marching Average)

The system uses a circular buffer to maintain the last 20 pressure samples. As each new sample arrives:
1. The oldest sample is removed from the buffer
2. The new sample is added
3. The average is recalculated

This "marching" or "rolling" average greatly smooths spikes in the data while remaining responsive to genuine trends.

### Baseline Pressure

Once 20 samples have been collected, the system establishes a baseline pressure representing typical conditions. This baseline slowly adapts over time to account for seasonal variations while remaining sensitive to rapid changes that indicate severe weather.

### Severe Weather Rules

Significant pressure changes warrant an on-screen notice:
- **Rapid Drop (>3 hPa)**: Indicates approaching storm system - seek shelter
- **Rapid Rise (>3 hPa)**: Indicates weather improvement - conditions stabilizing

These thresholds can be adjusted based on local conditions and requirements.

## Hardware Requirements

- Arduino board (Uno, Mega, ESP32, or similar)
- Atmospheric pressure sensor (BMP280, BME280, or equivalent)
- I2C connection for sensor communication

## Installation

1. Clone this repository
2. Open `WeatherStation_EmergencyData.ino` in Arduino IDE
3. Install required sensor libraries (e.g., Adafruit BMP280)
4. Connect your pressure sensor to the I2C pins
5. Upload to your Arduino board

## Files

- `WeatherStation_EmergencyData.ino` - Main sketch
- `MovingAverage.h` - Moving average filter implementation
- `PressureMonitor.h` - Pressure monitoring and alert system
- `test_moving_average.ino` - Test suite for moving average functionality
- `examples/PressureMonitoring/` - Complete working example

## Configuration

Edit these constants in the main sketch to customize behavior:

```cpp
const int SAMPLE_SIZE = 20;              // Number of samples for moving average
const float PRESSURE_THRESHOLD = 3.0;    // hPa threshold for alerts
const unsigned long SAMPLE_INTERVAL = 60000; // Sample every 60 seconds
```

## Usage

### Basic Example

```cpp
#include "MovingAverage.h"
#include "PressureMonitor.h"

MovingAverage pressureAvg(20);
PressureMonitor monitor(3.0);

void loop() {
  float rawPressure = readSensor();
  pressureAvg.addSample(rawPressure);
  float smoothed = pressureAvg.getAverage();
  
  monitor.updatePressure(smoothed);
  if (monitor.isSevereWeatherDetected()) {
    // Alert user
  }
}
```

### Running Tests

To test the moving average implementation:
1. Rename `test_moving_average.ino` to a sketch folder
2. Upload to your Arduino
3. Open Serial Monitor (9600 baud) to view test results

## Algorithm Details

### Moving Average Calculation

The moving average is calculated using a circular buffer:

```
Average = Sum of all samples / Number of samples
```

When the buffer is full (20 samples), adding a new sample:
1. Subtracts the oldest value from the running sum
2. Adds the new value to the running sum
3. Moves the index forward (wraps around at buffer end)

This provides O(1) time complexity for each update.

### Pressure Change Detection

Pressure change is calculated as:

```
Change = Current Smoothed Pressure - Baseline Pressure
```

An alert is triggered when: `|Change| >= Threshold`

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

This project is open source and available for use in emergency preparedness applications.
It's easy enough to get weather data online, and when to know when it's coming straight to you. But what about when it's on top of you? When is it safe to come out? This project aims to collect data useful in identifying when weather has normalized enough to leave shelter and return home.

## Overview

This ESP32-based weather station monitors environmental conditions to help determine when it's safe to leave shelter after severe weather events. The system tracks multiple parameters:

- **Air Pressure** (Primary Tool): Monitors atmospheric pressure changes that indicate weather pattern shifts
- **CO2 Levels** (Safety Flag): Detects dangerous CO2 buildup indicating poor ventilation or overcrowding
- **Temperature & Humidity** (Comfort): Tracks environmental comfort conditions

## Hardware Requirements

### ESP32 Development Board
- Any ESP32 board (ESP32-DevKit, NodeMCU-32S, etc.)

### Sensors
1. **BME280** - Temperature, Humidity, and Pressure sensor (I2C)
   - Address: 0x76 or 0x77
   
2. **SCD30** - CO2, Temperature, and Humidity sensor (I2C)
   - Address: 0x61

### Wiring Connections

```
ESP32          BME280/SCD30
-----          ------------
3.3V    -->    VCC
GND     -->    GND
GPIO21  -->    SDA
GPIO22  -->    SCL
```

## Software Requirements

### Development Environment
- **Visual Studio Code**
- **PlatformIO IDE Extension**

### Libraries (Automatically Managed)
The following libraries are automatically downloaded by PlatformIO:
- Adafruit Unified Sensor
- Adafruit BME280 Library
- Adafruit BusIO
- SparkFun SCD30 Arduino Library

All libraries are managed through `platformio.ini` and will be downloaded during the first build.

## Installation & Setup

1. **Install Visual Studio Code**
   - Download from: https://code.visualstudio.com/

2. **Install PlatformIO IDE Extension**
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. **Clone this Repository**
   ```bash
   git clone https://github.com/rflulling/WeatherStation_EmergencyData.git
   cd WeatherStation_EmergencyData
   ```

4. **Open in VS Code**
   ```bash
   code .
   ```

5. **Build the Project**
   - Click the PlatformIO icon in the left sidebar
   - Click "Build" or use shortcut: Ctrl+Alt+B

6. **Upload to ESP32**
   - Connect your ESP32 board via USB
   - Click "Upload" in PlatformIO or use: Ctrl+Alt+U

7. **Monitor Serial Output**
   - Click "Serial Monitor" in PlatformIO
   - Baud rate: 115200

## Usage

Once uploaded and running, the device will:

1. Initialize I2C communication
2. Detect and configure connected sensors
3. Begin continuous monitoring every 5 seconds
4. Display readings via Serial Monitor including:
   - Temperature (°C)
   - Humidity (%)
   - Atmospheric Pressure (hPa)
   - Altitude (meters)
   - CO2 Level (ppm)
   - Status indicators for pressure and CO2

### Interpreting Results

**Pressure Analysis:**
- High Pressure (>1020 hPa): Clear/Improving Weather
- Normal Pressure (1000-1020 hPa): Stable Conditions
- Low Pressure (<1000 hPa): Storm/Unstable Weather

**CO2 Analysis:**
- Normal (<1000 ppm): Safe air quality
- Warning (1000-2000 ppm): Poor ventilation or crowding
- Danger (>2000 ppm): Evacuate or ventilate immediately

## Project Structure

```
WeatherStation_EmergencyData/
├── src/
│   └── main.cpp              # Main application code
├── include/                  # Header files (if needed)
├── lib/                      # Local libraries (if needed)
├── test/                     # Test files
├── platformio.ini           # PlatformIO configuration
├── .gitignore              # Git ignore rules
└── README.md               # This file
```

## Configuration

### Sensor Addresses
If your sensors use different I2C addresses, modify in `src/main.cpp`:
```cpp
// BME280 addresses: 0x76 or 0x77
bme.begin(0x76);

// SCD30 default: 0x61
```

### Threshold Values
Adjust thresholds in `src/main.cpp`:
```cpp
#define CO2_WARNING_LEVEL 1000  // ppm
#define CO2_DANGER_LEVEL 2000   // ppm
#define SEALEVEL_PRESSURE_HPA (1013.25)
```

## Troubleshooting

**No sensors detected:**
- Check wiring connections
- Verify I2C addresses with an I2C scanner
- Ensure 3.3V power supply is adequate

**Build errors:**
- Clean the build: PlatformIO > Clean
- Rebuild: PlatformIO > Build

**Upload errors:**
- Check USB connection
- Verify correct COM port in Device Manager (Windows) or /dev/tty* (Linux/Mac)
- Try holding BOOT button on ESP32 during upload

## Future Enhancements

- Add pressure trend tracking over time
- Implement data logging to SD card
- Add WiFi connectivity for remote monitoring
- Create web interface for real-time display
- Add battery backup for continuous monitoring
- Implement alerting system (buzzer/LED)

## License

This project is open source. Feel free to use and modify for your needs.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

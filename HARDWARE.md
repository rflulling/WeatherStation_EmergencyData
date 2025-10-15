# Hardware Setup and Wiring Guide

## ESP32 Pinout Reference

For this project, we use the following ESP32 pins:

| Pin | Function | Description |
|-----|----------|-------------|
| GPIO21 | SDA | I2C Data Line |
| GPIO22 | SCL | I2C Clock Line |
| 3.3V | Power | Power supply for sensors |
| GND | Ground | Common ground |

## Sensor Wiring

### BME280 Sensor
The BME280 measures temperature, humidity, and atmospheric pressure.

```
ESP32          BME280
-----          ------
3.3V    -->    VCC (or VIN)
GND     -->    GND
GPIO21  -->    SDA (SDI)
GPIO22  -->    SCL (SCK)
```

**I2C Address:** 0x76 or 0x77 (configurable via jumper on some boards)

### SCD30 Sensor
The SCD30 measures CO2 concentration in the air.

```
ESP32          SCD30
-----          -----
3.3V    -->    VCC
GND     -->    GND
GPIO21  -->    SDA
GPIO22  -->    SCL
```

**I2C Address:** 0x61 (default, not configurable)

## Complete Connection Diagram

```
                    ESP32
                ┌──────────┐
                │          │
      ┌─────────┤ 3.3V     │
      │    ┌────┤ GND      │
      │    │ ┌──┤ GPIO21   │ (SDA)
      │    │ │┌─┤ GPIO22   │ (SCL)
      │    │ ││ │          │
      │    │ ││ └──────────┘
      │    │ ││
      │    │ ││   BME280          SCD30
      │    │ ││ ┌─────────┐    ┌─────────┐
      │    │ ││ │ VCC  3V3│    │VCC   3V3│
      ├────┼─┼┼─┤ GND  GND├────┤GND   GND│
      │    └─┼┼─┤ SCL  SCL├──┬─┤SCL   SCL│
      │      └┼─┤ SDA  SDA├┐ │ │SDA   SDA│
      │       └─┤         ││ │ │         │
      │         └─────────┘│ │ └─────────┘
      └─────────────────────┘ │
                              │
         I2C Bus (shared) ────┘
```

## Important Notes

1. **Power Supply**: Both sensors require 3.3V. DO NOT connect to 5V as it may damage the sensors.

2. **I2C Bus**: Multiple I2C devices share the same SDA and SCL lines. This is normal and expected.

3. **Pull-up Resistors**: Most ESP32 boards and sensor breakout boards have built-in pull-up resistors for I2C. If you're experiencing communication issues, you may need to add external 4.7kΩ pull-up resistors on SDA and SCL lines to 3.3V.

4. **Wire Length**: Keep I2C wires as short as possible (preferably under 20cm) for reliable communication.

5. **Power Consumption**: 
   - BME280: ~3.6μA (sleep mode), ~714μA (active)
   - SCD30: ~19mA (measurement mode)
   - Ensure your power supply can handle at least 50mA

## Verification Steps

After wiring:

1. Double-check all connections match the diagram
2. Verify no short circuits between power and ground
3. Connect ESP32 to computer via USB
4. Upload the code
5. Open Serial Monitor at 115200 baud
6. You should see initialization messages for both sensors

If sensors are not detected:
- Use an I2C scanner sketch to verify addresses
- Check for loose connections
- Verify power supply voltage
- Try one sensor at a time to isolate issues

## Alternative Sensors

If you want to use different sensors:

### Alternative CO2 Sensors:
- **SCD40/SCD41**: More compact than SCD30, similar functionality
- **MH-Z19B**: Lower cost but requires UART connection

### Alternative Environmental Sensors:
- **BME680**: Adds VOC gas sensing capability
- **BMP280**: Pressure and temperature only (no humidity)
- **DHT22**: Lower cost but less accurate for temperature/humidity

You would need to modify the code and library dependencies accordingly.

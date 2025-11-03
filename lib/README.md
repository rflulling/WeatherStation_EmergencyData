# Libraries

This folder is for project-specific custom libraries. 

All required third-party libraries are automatically managed by PlatformIO through the `platformio.ini` configuration file and will be downloaded during the first build.

## Included Libraries (via PlatformIO)

1. **Adafruit Unified Sensor** (v1.1.14)
   - Base library for Adafruit sensors
   
2. **Adafruit BME280 Library** (v2.2.4)
   - Driver for BME280 temperature, humidity, and pressure sensor
   
3. **Adafruit BusIO** (v1.16.1)
   - I2C/SPI communication library
   
4. **SparkFun SCD30 Arduino Library** (v1.0.19)
   - Driver for SCD30 CO2 sensor

## Adding Custom Libraries

If you need to add custom libraries:
1. Create a new folder in this directory with your library name
2. Add your library files (.h and .cpp)
3. The library will be automatically available to your project

## Library Documentation

- **BME280**: https://github.com/adafruit/Adafruit_BME280_Library
- **SCD30**: https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library

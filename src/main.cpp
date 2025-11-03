/**
 * Weather Station Emergency Data - ESP32
 * 
 * This project collects environmental data to help identify when weather
 * conditions have normalized enough after an emergency to leave shelter.
 * 
 * Sensors:
 * - BME280: Temperature, Humidity, and Pressure
 * - SCD30: CO2 sensor for air quality monitoring
 * 
 * CO2 levels indicate if there are too many people in a confined space.
 * Temperature and humidity provide comfort data.
 * Air pressure is the primary tool for weather normalization detection.
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFun_SCD30_Arduino_Library.h>

// I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// Sensor thresholds
#define CO2_WARNING_LEVEL 1000  // ppm - indicates too many people
#define CO2_DANGER_LEVEL 2000   // ppm - dangerous levels
#define SEALEVEL_PRESSURE_HPA (1013.25)

// Sensor objects
Adafruit_BME280 bme;
SCD30 airSensor;

// Global variables to store sensor readings
struct SensorData {
  float temperature;      // Celsius
  float humidity;         // %
  float pressure;         // hPa
  float altitude;         // meters
  uint16_t co2;          // ppm
  bool bme280_ok;
  bool scd30_ok;
};

SensorData currentData;

/**
 * Initialize I2C communication
 */
void initI2C() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("I2C initialized");
}

/**
 * Initialize BME280 sensor
 */
bool initBME280() {
  if (!bme.begin(0x76)) {
    // Try alternate address
    if (!bme.begin(0x77)) {
      Serial.println("Could not find BME280 sensor!");
      return false;
    }
  }
  
  Serial.println("BME280 sensor initialized");
  
  // Configure BME280 for weather monitoring
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X2,  // temperature
                  Adafruit_BME280::SAMPLING_X16, // pressure
                  Adafruit_BME280::SAMPLING_X1,  // humidity
                  Adafruit_BME280::FILTER_X16,
                  Adafruit_BME280::STANDBY_MS_0_5);
  
  return true;
}

/**
 * Initialize SCD30 CO2 sensor
 */
bool initSCD30() {
  if (!airSensor.begin()) {
    Serial.println("Could not find SCD30 sensor!");
    return false;
  }
  
  Serial.println("SCD30 sensor initialized");
  
  // Set measurement interval (2-1800 seconds)
  airSensor.setMeasurementInterval(2);
  
  // Enable automatic self-calibration
  airSensor.setAutoSelfCalibration(true);
  
  return true;
}

/**
 * Read data from BME280 sensor
 */
void readBME280() {
  if (currentData.bme280_ok) {
    currentData.temperature = bme.readTemperature();
    currentData.humidity = bme.readHumidity();
    currentData.pressure = bme.readPressure() / 100.0F;
    currentData.altitude = bme.readAltitude(SEALEVEL_PRESSURE_HPA);
  }
}

/**
 * Read data from SCD30 sensor
 */
void readSCD30() {
  if (currentData.scd30_ok && airSensor.dataAvailable()) {
    currentData.co2 = airSensor.getCO2();
  }
}

/**
 * Analyze CO2 levels and provide safety status
 */
String analyzeCO2() {
  if (!currentData.scd30_ok) {
    return "CO2 Sensor Offline";
  }
  
  if (currentData.co2 < CO2_WARNING_LEVEL) {
    return "CO2 Normal";
  } else if (currentData.co2 < CO2_DANGER_LEVEL) {
    return "CO2 WARNING - Too many people or poor ventilation";
  } else {
    return "CO2 DANGER - Evacuate or ventilate immediately!";
  }
}

/**
 * Analyze pressure trends for weather pattern detection
 * Note: For proper trend analysis, you would need to track
 * pressure changes over time (hours/days)
 */
String analyzePressure() {
  if (!currentData.bme280_ok) {
    return "Pressure Sensor Offline";
  }
  
  // Basic pressure analysis
  if (currentData.pressure > 1020.0) {
    return "High Pressure - Clear/Improving Weather";
  } else if (currentData.pressure < 1000.0) {
    return "Low Pressure - Storm/Unstable Weather";
  } else {
    return "Normal Pressure - Stable Conditions";
  }
}

/**
 * Display all sensor readings
 */
void displaySensorData() {
  Serial.println("\n========================================");
  Serial.println("    WEATHER STATION - EMERGENCY DATA");
  Serial.println("========================================");
  
  // BME280 Data
  if (currentData.bme280_ok) {
    Serial.println("\n--- Environmental Conditions ---");
    Serial.print("Temperature: ");
    Serial.print(currentData.temperature);
    Serial.println(" °C");
    
    Serial.print("Humidity: ");
    Serial.print(currentData.humidity);
    Serial.println(" %");
    
    Serial.print("Pressure: ");
    Serial.print(currentData.pressure);
    Serial.println(" hPa");
    
    Serial.print("Altitude: ");
    Serial.print(currentData.altitude);
    Serial.println(" m");
    
    Serial.print("Status: ");
    Serial.println(analyzePressure());
  } else {
    Serial.println("\n--- BME280 Sensor Offline ---");
  }
  
  // SCD30 Data
  if (currentData.scd30_ok) {
    Serial.println("\n--- Air Quality ---");
    Serial.print("CO2: ");
    Serial.print(currentData.co2);
    Serial.println(" ppm");
    
    Serial.print("Status: ");
    Serial.println(analyzeCO2());
  } else {
    Serial.println("\n--- SCD30 Sensor Offline ---");
  }
  
  Serial.println("========================================\n");
}

/**
 * Setup function - runs once at startup
 */
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("Weather Station Emergency Data");
  Serial.println("ESP32 Weather & Air Quality Monitor");
  Serial.println("========================================\n");
  
  // Initialize I2C
  initI2C();
  delay(100);
  
  // Initialize sensors
  currentData.bme280_ok = initBME280();
  delay(100);
  
  currentData.scd30_ok = initSCD30();
  delay(100);
  
  if (!currentData.bme280_ok && !currentData.scd30_ok) {
    Serial.println("\nERROR: No sensors detected!");
    Serial.println("Please check wiring and I2C connections.");
  } else {
    Serial.println("\nSensor initialization complete!");
    Serial.println("Starting measurements...\n");
  }
}

/**
 * Main loop - runs continuously
 */
void loop() {
  // Read sensor data
  readBME280();
  readSCD30();
  
  // Display results
  displaySensorData();
  
  // Wait 5 seconds before next reading
  delay(5000);
}

/*
 * test_moving_average.ino
 * 
 * Test program for the MovingAverage class
 * Demonstrates the moving average behavior with sample data
 */

#include "MovingAverage.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port
  }
  
  Serial.println("Moving Average Test");
  Serial.println("===================");
  Serial.println();
  
  // Test 1: Basic moving average with 5 samples
  Serial.println("Test 1: Basic moving average (window size = 5)");
  Serial.println("-----------------------------------------------");
  testBasicAverage();
  Serial.println();
  
  // Test 2: Moving average with 20 samples (as required)
  Serial.println("Test 2: Moving average with 20 samples");
  Serial.println("---------------------------------------");
  testTwentySampleAverage();
  Serial.println();
  
  // Test 3: Spike smoothing demonstration
  Serial.println("Test 3: Spike smoothing demonstration");
  Serial.println("--------------------------------------");
  testSpikeSmoothing();
  Serial.println();
  
  Serial.println("All tests completed!");
}

void loop() {
  // Tests run once in setup()
}

void testBasicAverage() {
  MovingAverage avg(5);
  
  float testData[] = {10.0, 12.0, 11.0, 13.0, 12.0, 14.0, 15.0, 13.0};
  int dataSize = sizeof(testData) / sizeof(testData[0]);
  
  for (int i = 0; i < dataSize; i++) {
    avg.addSample(testData[i]);
    Serial.print("Sample ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(testData[i], 2);
    Serial.print(" | Average: ");
    Serial.print(avg.getAverage(), 2);
    Serial.print(" | Count: ");
    Serial.print(avg.getSampleCount());
    Serial.print(" | Full: ");
    Serial.println(avg.isFull() ? "Yes" : "No");
  }
}

void testTwentySampleAverage() {
  MovingAverage avg(20);
  
  Serial.println("Adding 25 samples to demonstrate rolling window:");
  
  for (int i = 1; i <= 25; i++) {
    float value = 1000.0 + i; // Increasing values
    avg.addSample(value);
    
    Serial.print("Sample ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(value, 2);
    Serial.print(" | Average: ");
    Serial.print(avg.getAverage(), 2);
    
    if (avg.isFull()) {
      Serial.print(" | Buffer FULL - oldest sample replaced");
    }
    Serial.println();
  }
}

void testSpikeSmoothing() {
  MovingAverage avg(20);
  
  // Simulate steady pressure with occasional spikes
  float baseValue = 1013.25; // Standard sea level pressure
  
  Serial.println("Simulating pressure readings with spikes:");
  
  for (int i = 0; i < 30; i++) {
    float value = baseValue;
    
    // Add spikes at specific points
    if (i == 10 || i == 11) {
      value += 10.0; // Large positive spike
    } else if (i == 20) {
      value -= 8.0; // Large negative spike
    } else {
      // Add small random variation
      value += (random(-50, 50) / 100.0);
    }
    
    avg.addSample(value);
    
    Serial.print("Sample ");
    Serial.print(i + 1);
    Serial.print(": Raw = ");
    Serial.print(value, 2);
    Serial.print(" hPa | Smoothed = ");
    Serial.print(avg.getAverage(), 2);
    Serial.print(" hPa");
    
    if (i >= 10 && i <= 12) {
      Serial.print(" <-- Spike period");
    }
    if (i == 20) {
      Serial.print(" <-- Negative spike");
    }
    
    Serial.println();
  }
  
  Serial.println();
  Serial.println("Notice how the moving average smooths out the spikes!");
}

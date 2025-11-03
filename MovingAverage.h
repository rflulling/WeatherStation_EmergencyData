/*
 * MovingAverage.h
 * 
 * Implements a moving average filter with a circular buffer.
 * As new samples are added, the oldest sample is automatically
 * removed, creating a "marching" or "rolling" average.
 */

#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

class MovingAverage {
  private:
    float* buffer;        // Circular buffer for samples
    int bufferSize;       // Size of the buffer
    int currentIndex;     // Current position in buffer
    int sampleCount;      // Number of samples added so far
    float sum;            // Running sum of samples
    
  public:
    // Constructor
    MovingAverage(int size) {
      bufferSize = size;
      buffer = new float[bufferSize];
      currentIndex = 0;
      sampleCount = 0;
      sum = 0.0;
      
      // Initialize buffer
      for (int i = 0; i < bufferSize; i++) {
        buffer[i] = 0.0;
      }
    }
    
    // Destructor
    ~MovingAverage() {
      delete[] buffer;
    }
    
    // Add a new sample to the moving average
    void addSample(float value) {
      // Subtract the value being replaced from the sum
      if (sampleCount >= bufferSize) {
        sum -= buffer[currentIndex];
      }
      
      // Add new value to buffer and sum
      buffer[currentIndex] = value;
      sum += value;
      
      // Update index (wrap around using modulo)
      currentIndex = (currentIndex + 1) % bufferSize;
      
      // Track number of samples (up to buffer size)
      if (sampleCount < bufferSize) {
        sampleCount++;
      }
    }
    
    // Get the current average
    float getAverage() {
      if (sampleCount == 0) {
        return 0.0;
      }
      return sum / sampleCount;
    }
    
    // Check if buffer is full
    bool isFull() {
      return sampleCount >= bufferSize;
    }
    
    // Get the number of samples currently in the buffer
    int getSampleCount() {
      return sampleCount;
    }
    
    // Reset the moving average
    void reset() {
      currentIndex = 0;
      sampleCount = 0;
      sum = 0.0;
      
      for (int i = 0; i < bufferSize; i++) {
        buffer[i] = 0.0;
      }
    }
    
    // Get a specific sample from the buffer (for debugging)
    float getSample(int index) {
      if (index < 0 || index >= sampleCount) {
        return 0.0;
      }
      return buffer[index];
    }
};

#endif // MOVING_AVERAGE_H

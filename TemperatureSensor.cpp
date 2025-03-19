#include "TemperatureSensor.h"
#include <random>

TemperatureSensor::TemperatureSensor(const std::string& name, Server* server)
        : Sensor(name, "Temperature", server) {
    // Initialize the random number generator with a true random seed from the hardware
    std::random_device rd;
    generator = std::mt19937(rd());  // Initialize with the seed
    // Set up the distribution to generate random integers between -20 and 40 (representing temperature in degrees Celsius)
    distribution = std::uniform_int_distribution<int>(-20, 40);  // Temperature range from -20°C to 40°C
}

void TemperatureSensor::update() {
    // Generate a random temperature value within the defined range (-20°C to 40°C)
    int randomValue = distribution(generator);
    // Update the sensor's data by converting the random value to a string and adding the "degrees Celsius" unit
    setData(std::to_string(randomValue) + " (degrees Celsius)");
}

// Destructor for the TemperatureSensor class
TemperatureSensor::~TemperatureSensor() {}
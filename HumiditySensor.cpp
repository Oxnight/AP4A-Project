#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(const std::string& name, Server* server)
        : Sensor(name, "Humidity", server) {
    // Initialize random number generator with a true random seed from the hardware
    std::random_device rd;
    generator = std::mt19937(rd());  // Use a random generator based on hardware entropy
    // Set up the distribution to generate random integers between 0 and 100 (inclusive)
    distribution = std::uniform_int_distribution<int>(0, 100);  // Range for humidity from 0% to 100%
}

void HumiditySensor::update() {
    // Generate a random humidity value within the defined range (0-100)
    int randomValue = distribution(generator);
    // Update the sensor's data by converting the random value to a string and adding the '%' unit
    setData(std::to_string(randomValue) + " (%)");
}

// Destructor for the HumiditySensor class
HumiditySensor::~HumiditySensor() {}
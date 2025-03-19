#include "NoiseSensor.h"

NoiseSensor::NoiseSensor(const std::string& name, Server* server)
        : Sensor(name, "Noise", server) {
    // Initialize the random number generator with a true random seed from the hardware
    std::random_device rd;
    generator = std::mt19937(rd());  // Initialize with the seed
    // Set up the distribution to generate random integers between 30 and 130 (representing noise levels in dB)
    distribution = std::uniform_int_distribution<int>(30, 130);  // Noise level range in dB
}

void NoiseSensor::update() {
    // Generate a random noise value within the defined range (30-130 dB)
    int randomValue = distribution(generator);
    // Update the sensor's data by converting the random value to a string and adding the "dB" unit
    setData(std::to_string(randomValue) + " (dB)");
}

// Destructor for the NoiseSensor class
NoiseSensor::~NoiseSensor() {}
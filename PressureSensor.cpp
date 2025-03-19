#include "PressureSensor.h"

PressureSensor::PressureSensor(const std::string& name, Server* server)
        : Sensor(name, "Pressure", server) {
    // Initialize the random number generator with a true random seed from the hardware
    std::random_device rd;
    generator = std::mt19937(rd());  // Initialize with the seed
    // Set up the distribution to generate random integers between 95 and 105 (representing pressure in kPa)
    distribution = std::uniform_int_distribution<int>(95, 105);  // Pressure range from 95 kPa to 105 kPa
}

void PressureSensor::update() {
    // Generate a random pressure value within the defined range (95-105 kPa)
    int randomValue = distribution(generator);
    // Update the sensor's data by converting the random value to a string and adding the "kPa" unit
    setData(std::to_string(randomValue) + " (kPa)");
}

// Destructor for the PressureSensor class
PressureSensor::~PressureSensor() {}
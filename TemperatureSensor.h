#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"
#include <random>

class TemperatureSensor : public Sensor {
public:
    // Constructor initializes the temperature sensor with a name and a reference to the server
    TemperatureSensor(const std::string& name, Server* server);

    // Destructor
    virtual ~TemperatureSensor();

    // Override the update method to generate and set random temperature data
    void update() override;

    // Override the getUpdateInterval method to return the interval between updates
    std::chrono::seconds getUpdateInterval() const override {
        return std::chrono::seconds(1); // Set update interval to 1 second
    }

private:
    std::mt19937 generator;  // Random number generator for generating temperature values
    std::uniform_int_distribution<int> distribution;  // Defines the distribution range for temperature values (-20°C to 40°C)
};

#endif // TEMPERATURESENSOR_H
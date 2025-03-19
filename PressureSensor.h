#ifndef PROJET_PRESSURESENSOR_H
#define PROJET_PRESSURESENSOR_H

#include "Sensor.h"
#include <random>

class PressureSensor : public Sensor {
public:
    // Constructor initializes the pressure sensor with a name and a reference to the server
    PressureSensor(const std::string& name, Server* server);

    // Destructor
    virtual ~PressureSensor();

    // Override the update method to generate and set random pressure data
    void update() override;

    // Override the getUpdateInterval method to return the interval between updates
    std::chrono::seconds getUpdateInterval() const override {
        return std::chrono::seconds(2); // Set update interval to 2 seconds
    }

private:
    std::mt19937 generator;  // Random number generator for generating pressure values
    std::uniform_int_distribution<int> distribution;  // Defines the distribution range for pressure values (95-105 kPa)
};

#endif // PROJET_PRESSURESENSOR_H
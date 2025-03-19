#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

#include "Sensor.h"
#include <random>

class HumiditySensor : public Sensor {
public:
    // Constructor initializes the humidity sensor with a name and a reference to the server
    HumiditySensor(const std::string& name, Server* server);

    // Destructor
    virtual ~HumiditySensor();

    // Override the update method to generate and set random humidity data
    void update() override;

    // Override the getUpdateInterval method to return the interval between updates
    std::chrono::seconds getUpdateInterval() const override {
        return std::chrono::seconds(3); // Set update interval to 3 seconds
    }

private:
    std::mt19937 generator;  // Random number generator for generating humidity values
    std::uniform_int_distribution<int> distribution;  // Defines the distribution range for random values (0-100%)
};

#endif // HUMIDITYSENSOR_H
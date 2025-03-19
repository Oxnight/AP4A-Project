#ifndef PROJET_NOISESENSOR_H
#define PROJET_NOISESENSOR_H

#include "Sensor.h"
#include <random>

class NoiseSensor : public Sensor {
public:
    // Constructor initializes the noise sensor with a name and a reference to the server
    NoiseSensor(const std::string& name, Server* server);

    // Destructor
    virtual ~NoiseSensor();

    // Override the update method to generate and set random noise data
    void update() override;

    // Override the getUpdateInterval method to return the interval between updates
    std::chrono::seconds getUpdateInterval() const override {
        return std::chrono::seconds(5); // Set update interval to 5 seconds
    }

private:
    std::mt19937 generator;  // Random number generator for generating noise levels
    std::uniform_int_distribution<int> distribution;  // Defines the distribution range for noise levels (30-130 dB)
};

#endif // PROJET_NOISESENSOR_H
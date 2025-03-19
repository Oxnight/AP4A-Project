#ifndef PROJET_SERVER_H
#define PROJET_SERVER_H

#include "Sensor.h"
#include <string>
#include <iostream>

class Server {
public:
    // Default constructor
    Server();

    // Destructor
    ~Server();

    // Method to measure data from a sensor
    void measure(Sensor* sensor);

    // Method to display sensor data in the console
    void consoleWrite(const std::string& id, const std::string& name,
                      const std::string& type, const std::string& measurement,
                      const std::string& time, const std::string& date) const;

    // Method to write sensor data to a file (CSV format)
    void fileWrite(const std::string& id, const std::string& name,
                   const std::string& type, const std::string& measurement,
                   const std::string& time, const std::string& date) const;

    // Method to display averages in the console
    void displayAverages(const std::string& outputFile) const;

private:
    // Helper methods to retrieve the current time and date as strings
    std::string getCurrentTime() const;
    std::string getCurrentDate() const;
};

#endif // PROJET_SERVER_H
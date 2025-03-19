#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Sensor.h"
#include <atomic>
#include <thread>
#include <vector>

class Scheduler {
public:
    // Constructor
    Scheduler();

    // Destructor
    ~Scheduler();

    // Add a sensor to the scheduler
    void addSensor(Sensor* sensor);

    // Add a server to the scheduler
    void addServer(Server* server);

    // Start the simulation
    void simulation();

    // Stop the simulation
    void stop();

    // Output the scheduler's state
    friend std::ostream& operator<<(std::ostream& os, const Scheduler& scheduler);

    // Collect data from log files
    void collectDataFromLogs(const std::vector<std::string>& logFiles, const std::string& outputFile);

    // Calculate and write averages
    void calculateAndWriteAverages(const std::string& outputFile);

private:
    std::vector<Sensor*> sensors;         // List of sensors
    std::vector<std::thread> sensorThreads; // Sensor threads
    std::vector<std::string> sensorData;  // Collected data
    std::vector<std::string> logFiles;    // Log files
    Server* server;                      // Server pointer
    std::atomic<bool> stopSimulation;     // Stop flag
};

#endif // SCHEDULER_H

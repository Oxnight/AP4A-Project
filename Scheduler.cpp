#include "Scheduler.h"
#include "Server.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>

// Constructor
Scheduler::Scheduler() : server(nullptr), stopSimulation(false) {}

// Destructor: Stops simulation if running
Scheduler::~Scheduler() {
    stop();
}

// Add a sensor to the scheduler
void Scheduler::addSensor(Sensor* sensor) {
    if (sensor) {
        sensors.push_back(sensor);
    }
}

// Add a server to the scheduler
void Scheduler::addServer(Server* server) {
    this->server = server;
}

// Start the simulation with threads for each sensor
void Scheduler::simulation() {
    stopSimulation = false;
    std::cout << "Starting simulation with threads...\n\n";

    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::seconds(30);

    // Lambda function for running sensor in its own thread
    auto runSensor = [&](Sensor* sensor) {
        while (std::chrono::high_resolution_clock::now() < end) {
            sensor->execute();
            if (server) {
                server->measure(sensor);
            }
            std::this_thread::sleep_for(sensor->getUpdateInterval());
        }
    };

    // Create and start threads for each sensor
    std::vector<std::thread> threads;
    for (auto* sensor : sensors) {
        if (sensor) {
            threads.emplace_back(runSensor, sensor);
        }
    }

    // Join threads to wait for completion
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "\nSimulation finished.\n";

    // Collect data from logs and write averages
    collectDataFromLogs({"Temperature_log.csv", "Noise_log.csv", "Humidity_log.csv", "Pressure_log.csv"}, "output.txt");
    calculateAndWriteAverages("output.txt");
}

// Stop the simulation
void Scheduler::stop() {
    stopSimulation = true;
}

// Overload << operator for Scheduler class
std::ostream& operator<<(std::ostream& os, const Scheduler& scheduler) {
    os << "Scheduler data:\n";
    for (const auto* sensor : scheduler.sensors) {
        os << *sensor;
    }
    return os;
}

// Collect data from log files and store in an output file
void Scheduler::collectDataFromLogs(const std::vector<std::string>& logFiles, const std::string& outputFile) {
    std::cout << "\nOpening output file : " << outputFile << "\n";
    std::ofstream output(outputFile);

    if (!output.is_open()) {
        std::cerr << "Unable to open output file: " << outputFile << "\n";
        return;
    }

    bool dataCollected = false;
    std::map<std::string, std::vector<std::string>> sensorData;

    // Process log files
    for (const std::string& logFile : logFiles) {
        std::ifstream file(logFile);

        if (!file.is_open()) {
            std::cerr << "Unable to open log file: " << logFile << "\n";
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string id, name, type, measurement, time, date;

            // Read CSV file
            std::getline(ss, id, ',');
            std::getline(ss, name, ',');
            std::getline(ss, type, ',');
            std::getline(ss, measurement, ',');
            std::getline(ss, time, ',');
            std::getline(ss, date, ',');

            // Clean measurement value
            size_t pos = measurement.find(" ");
            if (pos != std::string::npos) {
                measurement = measurement.substr(0, pos);
            }

            // Store measurement data
            sensorData[type].push_back(measurement);
            dataCollected = true;
        }

        file.close();
    }

    // Write data to output file
    for (const auto& entry : sensorData) {
        const std::string& type = entry.first;
        const std::vector<std::string>& measurements = entry.second;

        output << type << " data: ";
        for (size_t i = 0; i < measurements.size(); ++i) {
            output << measurements[i];
            if (i != measurements.size() - 1) {
                output << ", ";
            }
        }
        output << "\n";
    }

    output.close();

    // Remove unnecessary lines in the output file
    std::ifstream file(outputFile);
    if (!file.is_open()) {
        std::cerr << "Unable to open output file for reading: " << outputFile << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    // Rewrite the output file without unnecessary lines
    std::ofstream outputFinal(outputFile);
    if (!outputFinal.is_open()) {
        std::cerr << "Unable to open output file for writing: " << outputFile << "\n";
        return;
    }

    for (const auto& l : lines) {
        if (l.find("Type data:") == std::string::npos) {
            outputFinal << l << "\n";
        }
    }

    outputFinal.close();

    if (dataCollected) {
        std::cout << "Data successfully collected in " << outputFile << "\n";
    } else {
        std::cerr << "No data was collected.\n";
    }
}

// Calculate averages from collected data and write to file
void Scheduler::calculateAndWriteAverages(const std::string& outputFile) {
    std::cout << "Calculation of averages and writing to SensorAverages.txt\n";

    std::map<std::string, std::vector<double>> sensorData;

    // Read collected data
    std::ifstream input(outputFile);
    if (!input.is_open()) {
        std::cerr << "Unable to open file for reading: " << outputFile << "\n";
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        std::string type, measurementStr;

        // Read sensor type and measurements
        std::getline(ss, type, ':');
        size_t pos = type.find(" data");
        if (pos != std::string::npos) {
            type = type.substr(0, pos);
        }

        std::getline(ss, measurementStr);

        std::stringstream valuesStream(measurementStr);
        std::string value;
        std::vector<double> measurements;

        while (std::getline(valuesStream, value, ',')) {
            try {
                double measurement = std::stod(value);
                measurements.push_back(measurement);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid measurement value: " << value << "\n";
            }
        }

        if (!measurements.empty()) {
            sensorData[type] = measurements;
        }
    }

    input.close();

    // Write averages to SensorAverages.txt
    std::ofstream output("SensorAverages.txt");
    if (!output.is_open()) {
        std::cerr << "Unable to open SensorAverages.txt for writing\n";
        return;
    }

    // Calculate and write averages
    for (const auto& entry : sensorData) {
        const std::string& type = entry.first;
        const std::vector<double>& measurements = entry.second;

        double sum = std::accumulate(measurements.begin(), measurements.end(), 0.0);
        double average = sum / measurements.size();

        average = std::round(average * 10.0) / 10.0;

        std::string unit;
        if (type == "Humidity") unit = " %";
        else if (type == "Noise") unit = " dB";
        else if (type == "Pressure") unit = " hPa";
        else if (type == "Temperature") unit = " °C";
        else unit = "";

        output << type << " average: " << average << unit << "\n";
    }

    output.close();
    std::cout << "Averages written to SensorAverages.txt\n";

    // Remove output file after calculation
    if (std::remove(outputFile.c_str()) != 0) {
        std::cerr << "Error deleting file " << outputFile << "\n";
    } else {
        std::cout << "File " << outputFile << " deleted successfully\n";
    }

}
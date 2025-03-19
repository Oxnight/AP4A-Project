#ifdef _WIN32
#include <windows.h>
#endif

#include "Server.h"
#include <fstream>
#include <iomanip>
#include <mutex>

// Mutex to ensure thread safety when printing to the console
std::mutex consoleMutex;

// Default Constructor
Server::Server() {}

// Destructor
Server::~Server() {}

// Method to measure and log data from a given sensor
void Server::measure(Sensor *sensor) {
    if (sensor) { // Ensure the sensor pointer is not null
        sensor->update(); // Call the sensor's update method to fetch the latest data

        // Prepare the collected data
        std::string measurement = sensor->getData();  // Retrieve the latest measurement from the sensor
        std::string id = std::to_string(
                sensor->getId()); // Convert the sensor ID (assumed to be an integer) to a string
        std::string name = sensor->getName();           // Get the sensor's name (assumed to be a string)
        std::string type = sensor->getType();           // Get the sensor's type (assumed to be a string)
        std::string time = getCurrentTime();            // Get the current time as a string
        std::string date = getCurrentDate();            // Get the current date as a string

        // Log the sensor data to both the console and a file
        consoleWrite(id, name, type, measurement, time, date); // Display the data in the console
        fileWrite(id, name, type, measurement, time, date);    // Write the data to a log file
    } else {
        std::cout << "No sensor provided." << std::endl; // Display an error if no sensor is provided
    }
}

// Function to print the sensor data to the console with colored output
void Server::consoleWrite(const std::string &id, const std::string &name,
                          const std::string &type, const std::string &measurement,
                          const std::string &time, const std::string &date) const {

    // Lock the mutex to ensure thread safety while printing to the console
    std::lock_guard<std::mutex> lock(consoleMutex);

#ifdef _WIN32
    // Windows-specific code to set console text colors using Windows API
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set text color for each data type and print corresponding values with fixed widths
    SetConsoleTextAttribute(hConsole, 15);  // White for ID
    std::cout << std::setw(5) << id << " | ";

    SetConsoleTextAttribute(hConsole, 11);  // Cyan for name
    std::cout << std::setw(10) << name << " | ";

    SetConsoleTextAttribute(hConsole, 14);  // Yellow for type
    std::cout << std::setw(5) << type << " | ";

    SetConsoleTextAttribute(hConsole, 10);  // Green for measurement
    std::cout << std::setw(5) << measurement << " | ";

    SetConsoleTextAttribute(hConsole, 13);  // Magenta for time
    std::cout << std::setw(8) << time << " | ";

    SetConsoleTextAttribute(hConsole, 12);  // Red for date
    std::cout << std::setw(10) << date << std::endl;

    // Reset console text color to default
    SetConsoleTextAttribute(hConsole, 7);
#else
    // Linux/Mac-specific code using ANSI escape codes for colored text output
    std::cout << "\033[37m" << std::setw(5) << id << " | "  // White for ID
              << "\033[36m" << std::setw(10) << name << " | "  // Cyan for name
              << "\033[33m" << std::setw(5) << type << " | "  // Yellow for type
              << "\033[32m" << std::setw(5) << measurement << " | "  // Green for measurement
              << "\033[35m" << std::setw(8) << time << " | "  // Magenta for time
              << "\033[31m" << std::setw(10) << date << "\033[0m" << std::endl;  // Red for date, followed by reset
#endif
}

// Function to write the sensor data to a CSV file
void Server::fileWrite(const std::string &id, const std::string &name,
                       const std::string &type, const std::string &measurement,
                       const std::string &time, const std::string &date) const {

    // Generate the log file name based on the sensor type (e.g., "temperature_log.csv")
    std::string filename = type + "_log.csv";
    std::ofstream logFile(filename, std::ios::app);  // Open the file in append mode

    // Check if the file opened successfully
    if (!logFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;  // Exit if the file cannot be opened
    }

    logFile.seekp(0, std::ios::end);  // Move the file pointer to the end

    // If the file is empty, write a header line with column names
    if (logFile.tellp() == 0) {
        logFile << "ID,Name,Type,Measurement,Time,Date" << std::endl;
    }

    // Write the collected sensor data in CSV format
    logFile << id << "," << name << "," << type << "," << measurement << ","
            << "[" << time << "],[" << date << "]" << std::endl;

    logFile.close();  // Close the file after writing the data
}

// Function to display the sensor averages stored in a file with colored output
void Server::displayAverages(const std::string& outputFile) const {
#ifdef _WIN32
    // Windows: Use the SetConsoleTextAttribute function to color the text in the console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get the handle to the standard output
    std::ifstream averagesFile(outputFile);  // Open the specified file for reading

    // Check if the file was successfully opened
    if (averagesFile.is_open()) {
        std::string line;
        std::cout << "\nSensor Averages :\n";  // Print a header message

        // Read the file line by line
        while (std::getline(averagesFile, line)) {
            // Replace "°C" with "degrees Celsius"
            size_t pos = line.find("°C");
            if (pos != std::string::npos) {
                line.replace(pos, 2, "degrees Celsius");
            }

            // Check for and remove any unwanted trailing characters
            if (line.back() == 'C') {
                line.pop_back();  // Remove the trailing 'C'
            }

            // Set the console text color to blue (9) for displaying "Sensor Average"
            SetConsoleTextAttribute(hConsole, 9);
            std::cout << line.substr(0, line.find(":") + 1);  // Print the "Sensor Averages" part

            // Set the console text color to a different color for the values and units
            SetConsoleTextAttribute(hConsole, 5);  // Purple for the values and units
            std::cout << line.substr(line.find(":") + 1) << std::endl;  // Print the data (e.g., value and unit)

            // Reset the console text color to the default (7)
            SetConsoleTextAttribute(hConsole, 7);
        }
        averagesFile.close();  // Close the file after reading all the lines
    } else {
        // Print an error message if the file could not be opened
        std::cerr << "Unable to open file for reading.\n";
    }
#else
    // Linux/Mac : Use ANSI escape codes to color the text in the console
    std::ifstream averagesFile(outputFile);  // Open the specified file for reading

    // Check if the file was successfully opened
    if (averagesFile.is_open()) {
        std::string line;
        std::cout << "\nSensor Averages :\n";  // Print a header message

        // Read the file line by line
        while (std::getline(averagesFile, line)) {
            // Replace "°C" with "degrees Celsius"
            size_t pos = line.find("°C");
            if (pos != std::string::npos) {
                line.replace(pos, 2, "degrees Celsius");
            }

            // Check for and remove any unwanted trailing characters
            if (line.back() == 'C') {
                line.pop_back();  // Remove the trailing 'C'
            }

            // Print the "Sensor Averages" part with colored text using ANSI escape code
            std::cout << "\033[34m" << line.substr(0, line.find(":") + 1);

            // Print the value and unit with a different color
            std::cout << "\033[35m" << line.substr(line.find(":") + 1) << "\033[0m" << std::endl;

        }
        averagesFile.close();  // Close the file after reading all the lines
    } else {
        // Print an error message if the file could not be opened
        std::cerr << "Unable to open file for reading.\n";
    }
#endif
}

// Helper method to get the current time as a string in "HH:MM:SS" format
std::string Server::getCurrentTime() const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");  // Format time as "HH:MM:SS"
    return oss.str();
}

// Helper method to get the current date as a string in "Day DD Mon" format
std::string Server::getCurrentDate() const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%a %d %b");  // Format date as "Day DD Mon"
    return oss.str();
}
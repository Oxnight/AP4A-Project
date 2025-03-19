#ifndef PROJET_SENSOR_H
#define PROJET_SENSOR_H

#include <chrono>
#include <iostream>

class Server; // Forward declaration of the Server class

class Sensor {
public:
    // Constructor to initialize the sensor with a name, type, and server reference.
    Sensor(const std::string& name, const std::string& type, Server* server);

    // Destructor
    virtual ~Sensor();

    // Copy constructor
    Sensor(const Sensor& other);

    // Assignment operator
    Sensor& operator=(const Sensor& other);

    // Pure virtual method to update sensor data (to be implemented in derived classes).
    virtual void update() = 0;

    // Executes the sensor update.
    void execute();

    // Pure virtual method to get the update interval (to be implemented in derived classes).
    virtual std::chrono::seconds getUpdateInterval() const = 0;

    // Getters for the sensor properties
    std::string getType() const;
    std::string getName() const;
    std::string getData() const;
    int getId() const;

    // Friend function to output the sensor details
    friend std::ostream& operator<<(std::ostream& os, const Sensor& sensor);

protected:
    // Protected method to set sensor data (to be used by derived classes)
    void setData(const std::string& newData);

private:
    static int nextInt;  // Static variable to generate unique sensor IDs
    int id;              // Unique sensor ID
    std::string name;    // Sensor name
    std::string type;    // Sensor type
    std::string data;    // Sensor data
    Server* server;      // Pointer to the associated server
};

#endif // PROJET_SENSOR_H
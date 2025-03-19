#include "Sensor.h"

// Constructor: Initializes the sensor with a unique ID, name, type, and server.
Sensor::Sensor(const std::string& name, const std::string& type, Server* server)
        : id(nextInt++), name(name), type(type), server(server), data("No Data") {}

// Destructor
Sensor::~Sensor() {
}

// Copy constructor
Sensor::Sensor(const Sensor& other)
        : id(other.id), name(other.name), type(other.type), data(other.data), server(other.server) {
}

// Assignment operator
Sensor& Sensor::operator=(const Sensor& other) {
    if (this != &other) {
        id = other.id;
        name = other.name;
        type = other.type;
        data = other.data;
        server = other.server;
    }
    return *this;
}

// Executes the sensor's update method to collect or process new data.
void Sensor::execute() {
    update();
}
// Returns the sensor's type (e.g., "Temperature", "Humidity").
std::string Sensor::getType() const {
    return type;
}

// Returns the sensor's name (e.g., "Room Temp", "Humidity Sensor").
std::string Sensor::getName() const {
    return name;
}

// Returns the sensor's current data (e.g., "25°C", "60%").
std::string Sensor::getData() const {
    return data;
}

// Returns the sensor's unique ID.
int Sensor::getId() const {
    return id;
}

// Overloads the output stream operator to print sensor details.
std::ostream& operator<<(std::ostream& os, const Sensor& sensor) {
    os << "Sensor ID: " << sensor.getId()
       << ", Type: " << sensor.getType()
       << ", Data: " << sensor.getData();
    return os;
}

// Sets the sensor's data to the given value.
void Sensor::setData(const std::string& newData) {
    data = newData;
}

// Static variable to assign unique IDs to sensors.
int Sensor::nextInt = 0;
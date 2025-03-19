#include "Scheduler.h"
#include "Server.h"
#include "TemperatureSensor.h"
#include "HumiditySensor.h"
#include "NoiseSensor.h"
#include "PressureSensor.h"

int main() {
    // Create a Server object
    Server* server = new Server();
    Scheduler scheduler;

    // Create sensor objects with unique names and associate them with the server
    TemperatureSensor tempSensor("HeatWaveSurfer", server);
    HumiditySensor humiditySensor("SoggyBottom", server);
    NoiseSensor noiseSensor("QuietBuster", server);
    PressureSensor pressureSensor("BreatheEasyBuddy", server);

    // Add the sensors to the scheduler for data collection
    scheduler.addSensor(&tempSensor);
    scheduler.addSensor(&humiditySensor);
    scheduler.addSensor(&noiseSensor);
    scheduler.addSensor(&pressureSensor);
    scheduler.addServer(server);

    // Start the simulation of sensor data collection and processing
    scheduler.simulation();

    // Stop the simulation after the necessary data collection is complete
    scheduler.stop();

    // Display the averages of sensor readings stored in a file
    server->displayAverages("SensorAverages.txt");

    // Free dynamically allocated memory for the server
    delete server;

    return 0;
}
cmake_minimum_required(VERSION 3.29)
project(Projet)

set(CMAKE_CXX_STANDARD 17)

add_executable(Projet main.cpp
        Server.cpp
        Server.h
        Scheduler.cpp
        Scheduler.h
        Sensor.cpp
        Sensor.h
        TemperatureSensor.cpp
        TemperatureSensor.h
        HumiditySensor.cpp
        HumiditySensor.h
        NoiseSensor.cpp
        NoiseSensor.h
        PressureSensor.cpp
        PressureSensor.h)

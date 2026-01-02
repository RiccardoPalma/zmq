#include <iostream>
#include <string>
#include "zmq_bus.hpp"
#include <thread>
#include <chrono>

int main() {
    ZmqClient zmq;

    // Simula la pubblicazione di messaggi
    zmq.publish("temperature", "25 gradi");
    zmq.publish("camera", "video ok");

    return 0;
}

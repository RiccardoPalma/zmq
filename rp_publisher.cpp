#include <iostream>
#include <string>
#include "zmq_bus.hpp"
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <topic> <message>\n";
        return 1;
    }
    ZmqClient zmq;

    // Simula la pubblicazione di messaggi
    zmq.publish(argv[1], argv[2]);
    return 0;
}

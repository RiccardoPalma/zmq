#include <iostream>
#include "zmq_bus.hpp"

int main() {
    ZmqClient zmq;
    zmq.subscribe("temperature", [](const std::string& msg) {
        std::cout << "Ricevuto su 'temperature': " << msg << std::endl;
    });
    while(true) {
        // Loop infinito per mantenere il subscriber attivo
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}


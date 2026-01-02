#include <iostream>
#include <string>
#include "zmq.hpp"
#include <thread>
#include <chrono>

int main() {
    zmq::context_t ctx;
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    
    // Bind al protocollo TCP
    publisher.bind("tcp://127.0.0.1:5555");

    // inserire una breve pausa per permettere ai subscriber di connettersi
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "Publisher pronto, invio messaggi..." << std::endl;

    for (int i = 0; i < 5; ++i) {
        // Pubblica un messaggio su un topic
        publisher.send(zmq::buffer("temperature 25 gradi"), zmq::send_flags::none);
        std::string dato = "video ok";
        publisher.send(zmq::buffer("camera " + dato), zmq::send_flags::none);
    }

    return 0;
}

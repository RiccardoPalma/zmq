#include <iostream>
#include "zmq.hpp"

int main() {
    zmq::context_t ctx;
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.connect("tcp://127.0.0.1:5555");

    subscriber.set(zmq::sockopt::subscribe, "camera");

    for (int i = 0; i < 5; ++i) {
        zmq::message_t msg;
        subscriber.recv(msg, zmq::recv_flags::none);

        std::string data(static_cast<char*>(msg.data()), msg.size());
        std::cout << "Ricevuto: " << data << std::endl;
    }

    return 0;
}

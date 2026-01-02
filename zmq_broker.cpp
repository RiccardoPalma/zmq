#include <zmq.hpp>
#include <csignal>
#include <atomic>
#include <iostream>

static std::atomic<bool> running{true};
void signal_handler(int){ running = false; }

int main() {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    zmq::context_t ctx(1);
    zmq::socket_t xsub(ctx, zmq::socket_type::xsub);
    zmq::socket_t xpub(ctx, zmq::socket_type::xpub);

    xsub.bind("tcp://127.0.0.1:5555"); // ingress: PUB
    xpub.bind("tcp://127.0.0.1:5556"); // egress: SUB

    std::cout << "[ZMQ] Broker started\n";

    zmq::proxy(xsub, xpub);

    std::cout << "[ZMQ] Broker stopped\n";
    return 0;
}

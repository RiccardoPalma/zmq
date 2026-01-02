#include <zmq.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <topic>\n";
        std::cout << "Use 'all' as wildcard to match all topics\n";
        return 1;
    }

    std::string topic = argv[1];

    // ZeroMQ context e socket SUB
    zmq::context_t ctx(1);
    zmq::socket_t sub(ctx, zmq::socket_type::sub);

    // Connect al broker XPUB
    sub.connect("tcp://127.0.0.1:5556");

    // Imposta subscription
    if(topic == "all") {
        sub.set(zmq::sockopt::subscribe, "");
    } else {
        sub.set(zmq::sockopt::subscribe, topic);
    }

    std::cout << "[Sniffer] Listening on topic: " << topic << "\n";

    while (true) {
        zmq::message_t msg;
        try {
            sub.recv(msg, zmq::recv_flags::none);
        } catch(const zmq::error_t& e) {
            std::cerr << "ZMQ error: " << e.what() << "\n";
            continue;
        }

        std::string s(static_cast<char*>(msg.data()), msg.size());

        // Estraggo topic e payload (prima parola = topic)
        auto pos = s.find(' ');
        std::string recv_topic = (pos == std::string::npos ? s : s.substr(0, pos));
        std::string payload    = (pos == std::string::npos ? "" : s.substr(pos + 1));

        std::cout << "[" << recv_topic << "] " << payload << "\n";
    }

    return 0;
}

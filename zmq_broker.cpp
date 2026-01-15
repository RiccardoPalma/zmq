#include <csignal>
#include <iostream>
#include <zmq.hpp>

zmq::context_t ctx(1);

void signal_handler(int) { ctx.shutdown(); }

int main() {
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  zmq::socket_t xsub(ctx, zmq::socket_type::xsub);
  zmq::socket_t xpub(ctx, zmq::socket_type::xpub);

  xsub.bind("tcp://127.0.0.1:5555");
  xpub.bind("tcp://127.0.0.1:5556");

  std::cout << "[ZMQ] Broker started\n";

  try {
    zmq::proxy(xsub, xpub);
  } catch (const zmq::error_t &e) {
    if (e.num() == EINTR) {
      std::cout << "\n[ZMQ] Broker interrupted\n";
    } else {
      // crash and restart
    }
  }

  std::cout << "[ZMQ] Broker stopped\n";
  return 0;
}

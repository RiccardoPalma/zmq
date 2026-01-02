#pragma once
#include <zmq.hpp>
#include <thread>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <atomic>
#include <string>
#include <chrono>

class ZmqClient {
public:
    using Callback = std::function<void(const std::string&)>;

    ZmqClient(const std::string& pub_endpoint="tcp://127.0.0.1:5555",
              const std::string& sub_endpoint="tcp://127.0.0.1:5556")
        : ctx(1), pub(ctx, zmq::socket_type::pub), sub(ctx, zmq::socket_type::sub),
          running(true)
    {
        pub.connect(pub_endpoint);
        sub.connect(sub_endpoint);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // attesa connessione

        receiver = std::thread([this](){ recv_loop(); });
    }

    ~ZmqClient() {
        running = false;
        if(receiver.joinable()) receiver.join();
        pub.close();
        sub.close();
        ctx.close();
    }

    void publish(const std::string& topic, const std::string& payload) {
        std::lock_guard<std::mutex> lk(pub_mtx);
        std::string msg = topic + " " + payload;
        zmq::message_t m(msg.data(), msg.size());
        pub.send(m, zmq::send_flags::none);
    }

    void subscribe(const std::string& topic, Callback cb) {
        {
            std::lock_guard<std::mutex> lk(cb_mtx);
            callbacks[topic] = cb;
        }
        sub.set(zmq::sockopt::subscribe, topic);
    }

private:
    zmq::context_t ctx;
    zmq::socket_t pub;
    zmq::socket_t sub;
    std::thread receiver;
    std::atomic<bool> running;

    std::mutex pub_mtx;
    std::unordered_map<std::string, Callback> callbacks;
    std::mutex cb_mtx;

    void recv_loop() {
        zmq::pollitem_t items[] = { { sub, 0, ZMQ_POLLIN, 0 } };
        while (running) {
            zmq::poll(items, 1, 100);
            if(items[0].revents & ZMQ_POLLIN) {
                zmq::message_t msg;
                sub.recv(msg, zmq::recv_flags::none);
                std::string s(static_cast<char*>(msg.data()), msg.size());
                auto pos = s.find(' ');
                std::string topic = (pos==std::string::npos ? s : s.substr(0,pos));
                std::string payload = (pos==std::string::npos ? "" : s.substr(pos+1));

                Callback cb;
                {
                    std::lock_guard<std::mutex> lk(cb_mtx);
                    auto it = callbacks.find(topic);
                    if(it != callbacks.end()) cb = it->second;
                }
                if(cb) cb(payload);
            }
        }
    }
};

cmake_minimum_required(VERSION 3.10)
project(zmq_tcp_json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cerca ZeroMQ tramite pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Include directory per cppzmq (header-only)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cppzmq)
include_directories(${ZMQ_INCLUDE_DIRS})

# Aggiungi eseguibili per i test
add_executable(test_pub test/pub.cpp)
target_link_libraries(test_pub ${ZMQ_LIBRARIES})

add_executable(test_suba test/sub_a.cpp)
target_link_libraries(test_suba ${ZMQ_LIBRARIES})

add_executable(test_subb test/sub_b.cpp)
target_link_libraries(test_subb ${ZMQ_LIBRARIES})

add_executable(broker zmq_broker.cpp)
target_link_libraries(broker ${ZMQ_LIBRARIES})

add_executable(sniffer rp_sniffer.cpp)
target_link_libraries(sniffer ${ZMQ_LIBRARIES})

add_executable(publisher rp_publisher.cpp)
target_link_libraries(publisher ${ZMQ_LIBRARIES})
cmake_minimum_required(VERSION 3.10)
project(zmq_tcp_json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cerca ZeroMQ tramite pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Include directory per cppzmq (header-only)
# Assumendo che tu abbia clonato cppzmq dentro zmq_tcp_json/cppzmq
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cppzmq)
include_directories(${ZMQ_INCLUDE_DIRS})

# Aggiungi eseguibili per i test
add_executable(publisher test/publisher.cpp)
target_link_libraries(publisher ${ZMQ_LIBRARIES})

add_executable(sub_a test/sub_a.cpp)
target_link_libraries(sub_a ${ZMQ_LIBRARIES})

add_executable(sub_b test/sub_b.cpp)
target_link_libraries(sub_b ${ZMQ_LIBRARIES})

# Aggiungi eseguibile per l'app principale
add_executable(zmq_wrapper zmq_wrapper.cpp)
cmake_minimum_required(VERSION 3.5)

project(TestBoost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM ../../../../../Boost/boost_1_82_0)
set(BOOST_INCLUDE_DIR "C:/Program Files/Boost/boost_1_82_0/boost")
set(BOOST_LIB_DIR "C:/Program Files/Boost/boost_1_82_0/libs")

add_executable(Client client.cpp)
add_executable(Server server.cpp)


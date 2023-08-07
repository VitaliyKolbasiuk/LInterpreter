cmake_minimum_required(VERSION 3.5)

project(TestBoost LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    include_directories(SYSTEM /usr/local/include)
    set(CMAKE_PREFIX_PATH "~/Qt/6.2.4/macos")
    set(CMAKE_PREFIX_PATH "~/Qt/6.4.0/macos")
    set(BOOST_INCLUDE_DIR "/usr/local/include")
    set(BOOST_LIB_DIR "/usr/local/lib")
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
    set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib")
    
    # for -lcrypto (I do not know who uses it!)
    link_directories(/usr/local/opt/openssl/lib)
else()
    include_directories(SYSTEM ../../../../../Boost/boost_1_82_0)
    set(BOOST_INCLUDE_DIR "C:/Program Files/Boost/boost_1_82_0/boost")
    set(BOOST_LIB_DIR "C:/Program Files/Boost/boost_1_82_0/libs")
endif()

add_executable(Client client.cpp)
add_executable(Server server.cpp)


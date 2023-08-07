#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

int main() {
    try {
        io_context ioContext;
        tcp::socket socket(ioContext);

        // Connect to the server
        socket.connect(tcp::endpoint(ip::address::from_string("127.0.0.1"), 12345));

        // Send a message to the server
        std::string message = "Hello from client!";
        write(socket, buffer(message));

        // Receive the response from the server
        char data[1024];
        size_t len = socket.read_some(buffer(data));

        std::cout << "Received from server: " << data << std::endl;

        // Close the connection
        socket.close();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

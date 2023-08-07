#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;

void handle_client(tcp::socket&& socket) {
    try {
        // Read data from the client
        char data[1024];
        size_t len = socket.read_some(buffer(data));
        std::cout << "Received: " << data << std::endl;

        // Send a response back to the client
        std::string response = "Hello from server!";
        write(socket, buffer(response));
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

int main() {
    io_context ioContext;
    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 12345));

    std::cout << "Server started. Listening on port 12345..." << std::endl;

    while (true) {
        tcp::socket socket(ioContext);
        acceptor.accept(socket);

        std::cout << "Client connected: " << socket.remote_endpoint() << std::endl;

        // Start a new thread to handle the client connection
        std::thread(handle_client, std::move(socket)).detach();
    }

    return 0;
}

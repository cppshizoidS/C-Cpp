// default
#include <iostream>
#include <string>

// conan
#include "boost/asio.hpp"

using namespace boost::asio;


int main(int argc, char** argv)
{
    using boost::asio::ip::tcp;
    using boost::system::error_code;

    int port        = atoi(argv[2]);
    std::string msg = argv[3];
    const char* ip  = argv[1];
    io_context context;
    error_code error;
    tcp::endpoint ep(ip::address::from_string(ip), port);
    /* tcp::resolver resolver(context); */
    /* tcp::resolver::results_type endpoints = */
    /*     resolver.resolve(ip, "emm", error); */

    if (error) {
        std::cout << "error: " << error.what() << std::endl;
        return -1;
    }

    tcp::socket socket(context);

    std::cout << "connecting to server..." << std::endl;
    socket.connect(ep, error);
    if (error) {
        std::cout << "error: " << error.what() << std::endl;
        return -1;
    }
    /* connect(socket, endpoints); */

    std::cout << "sending msg: " << msg << std::endl;
    write(socket, buffer(msg), error);

    if (error) {
        std::cout << "error: " << error.what() << std::endl;
        return -1;
    }

    return 0;
}


#include <iostream>
#include <boost/asio.hpp>
#include "tcp_server.h"




int main() {
    std::cout << "Hello, World!" << std::endl;
    
    try{

        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();

    } catch (std::exception& e){

    	std::cerr << e.what() << std::endl;

    }

    
    return 0;
}




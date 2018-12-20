//
// Created by ferrum on 21/11/18.
//

#include "tcp_connection.h"


std::string make_daytime_string() {
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

tcp_connection::pointer tcp_connection::create(boost::asio::io_context& io_context) {
    return pointer(new tcp_connection(io_context));
}

boost::asio::ip::tcp::socket& tcp_connection::socket() {
    return socket_;
}

void tcp_connection::start() {


    socket_.read_some(boost::asio::buffer(data_buffer));

    std::cout << data_buffer << std::endl;

    message_ = make_daytime_string();

    socket_.write_some(boost::asio::buffer(message_));
}

void tcp_connection::read_handler(
        const boost::system::error_code& error, // Result of operation.
        std::size_t bytes_transferred           // Number of bytes read.
){
    std::cout << bytes_transferred << std::endl;
}



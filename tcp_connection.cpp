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
    message_ = make_daytime_string();

    boost::asio::async_write(
            socket_,
            boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)
            );
}



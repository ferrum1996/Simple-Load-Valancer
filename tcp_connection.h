//
// Created by ferrum on 21/11/18.
//

#ifndef SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H
#define SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <ctime>
#include <boost/bind.hpp>
#include <iostream>

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>{

public:

    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context);

    boost::asio::ip::tcp::socket& socket();

    void start();

private:
    explicit tcp_connection(boost::asio::io_context& io_context)
            : socket_(io_context){}

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/){};

    void read_handler(const boost::system::error_code& error, std::size_t bytes_transferred );

    boost::asio::ip::tcp::socket socket_;
    std::string message_;
    char data_buffer[1024];

};


#endif //SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H

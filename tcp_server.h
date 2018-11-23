//
// Created by ferrum on 21/11/18.
//

#ifndef SIMPLE_LOAD_VALANCER_TCP_SERVER_H
#define SIMPLE_LOAD_VALANCER_TCP_SERVER_H


#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include "tcp_connection.h"

class tcp_server {
    public:
        tcp_server(boost::asio::io_context& io_context);

    private:
        void start_accept();

        void handle_accept (
                tcp_connection::pointer new_connection,
                const boost::system::error_code& error
        );

    boost::asio::ip::tcp::acceptor acceptor_;
};


#endif //SIMPLE_LOAD_VALANCER_TCP_SERVER_H

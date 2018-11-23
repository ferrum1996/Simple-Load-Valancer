#include "tcp_server.h"//
// Created by ferrum on 21/11/18.
//

#include "tcp_server.h"


tcp_server::tcp_server(boost::asio::io_context& io_context)
            :acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9000))
    {
        start_accept();
    }

void tcp_server::start_accept(){

    tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_executor().context());

    acceptor_.async_accept(new_connection->socket(), boost::bind(&tcp_server::handle_accept,
            this, new_connection, boost::asio::placeholders::error));

}

void tcp_server::handle_accept (
        tcp_connection::pointer new_connection,
        const boost::system::error_code& error
    ) {
        if(!error){
            new_connection->start();
        }

        start_accept();
    }


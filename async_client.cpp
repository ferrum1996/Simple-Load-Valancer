/**
    Created by BurningPapaya
    on 06/01/19 19:51 
**/

//
// Created by ferrum on 06/01/19.
//

#include "async_client.h"

void
async_client::run(
        char const* host,
        char const* port,
        char const* target,
        int version)
{
    // Set up an HTTP GET request message
    request_.version(11);
    request_.method(http::verb::get);
    request_.target(target);
    request_.set(http::field::host, host);
    request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Look up the domain name
    resolver_.async_resolve(
            host,
            port,
            std::bind(
                    &async_client::on_resolve,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2));
}

void
async_client::on_resolve(
        beast::error_code ec,
        tcp::resolver::results_type results)
{
    if(ec)
        return fail(ec, "resolve");

    // Make the connection on the IP address we get from a lookup
    net::async_connect(
            socket_,
            results.begin(),
            results.end(),
            std::bind(
                    &async_client::on_connect,
                    shared_from_this(),
                    std::placeholders::_1));
}

void
async_client::on_connect(beast::error_code ec)
{
    if(ec)
        return fail(ec, "connect");

    // Send the HTTP request to the remote host
    http::async_write(socket_, request_,
                      std::bind(
                              &async_client::on_write,
                              shared_from_this(),
                              std::placeholders::_1,
                              std::placeholders::_2));
}


void
async_client::on_write(
        beast::error_code ec,
        std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "write");

    // Receive the HTTP response
    http::async_read(socket_, buffer_, response_,
                     std::bind(
                             &async_client::on_read,
                             shared_from_this(),
                             std::placeholders::_1,
                             std::placeholders::_2));
}

void
async_client::on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if(ec)
        return fail(ec, "read");

    // Write the message to standard out
    std::cout << response_ << std::endl;

    // Gracefully close the socket
    socket_.shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes so don't bother reporting it.
    if(ec && ec != beast::errc::not_connected)
        return fail(ec, "shutdown");

    // If we get here then the connection is closed gracefully
}

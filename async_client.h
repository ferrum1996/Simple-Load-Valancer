//
// Created by ferrum on 06/01/19.
//

#ifndef SIMPLE_LOAD_VALANCER_ASYNC_CLIENT_H
#define SIMPLE_LOAD_VALANCER_ASYNC_CLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

// Performs an HTTP GET and prints the response
class async_client : public std::enable_shared_from_this<async_client>
{
    
private: 
    tcp::resolver resolver_;
    tcp::socket socket_;
    beast::flat_buffer buffer_; // (Must persist between reads)
    http::request<http::empty_body> request_;
    http::response<http::string_body> response_;

public:
    // Resolver and socket require an io_context
    explicit
    async_client(net::io_context& ioc)
            : resolver_(ioc)
            , socket_(ioc)
    {
    }

    // Start the asynchronous operation
    void
    run(
            char const* host,
            char const* port,
            char const* target,
            int version);

    void
    on_resolve(
            beast::error_code ec,
            tcp::resolver::results_type results);
    void
    on_connect(beast::error_code ec);

    void
    on_write(
            beast::error_code ec,
            std::size_t bytes_transferred);

    void
    on_read(
            beast::error_code ec,
            std::size_t bytes_transferred);
};






#endif //SIMPLE_LOAD_VALANCER_ASYNC_CLIENT_H

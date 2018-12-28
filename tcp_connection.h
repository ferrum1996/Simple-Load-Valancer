//
// Created by ferrum on 21/11/18.
//

#ifndef SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H
#define SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <ctime>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>{

public:

    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(asio::io_context& io_context);

    tcp::socket& socket();

    void start();

private:
    explicit tcp_connection(asio::io_context& io_context)
            : socket_(io_context){};


    template<class Body, class Allocator, class Send>
    void handle_request (
            http::request<Body, http::basic_fields<Allocator>>&& request,
            Send&& send)
    {
        beast::error_code ec;
        http::string_body::value_type body;

        auto const size = body.size();

        // Respond to request
        http::response<http::string_body> response{http::status::ok, request.version()};
        response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        response.set(http::field::content_type, "application/json");
        response.body()= R"({"message":"HELLO MESSAGE"})";
        response.prepare_payload();
        response.keep_alive(request.keep_alive());
        return send(std::move(response));
    }

    //lambda to send responses
    template<class Stream> struct send_lambda
    {
        Stream& stream_;
        bool& close_;
        beast::error_code& ec_;

        explicit send_lambda(
                Stream& stream,
                bool& close,
                beast::error_code& ec)
                : stream_(stream)
                , close_(close)
                , ec_(ec)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
        operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // Determine if we should close the connection after
            close_ = msg.need_eof();

            // We need the serializer here because the serializer requires
            // a non-const file_body, and the message oriented version of
            // http::write only works with const messages.
            http::serializer<isRequest, Body, Fields> sr{msg};
            http::write(stream_, sr, ec_);
        }
    };

    tcp::socket socket_;

};


#endif //SIMPLE_LOAD_VALANCER_TCP_CONNECTION_H

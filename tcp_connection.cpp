//
// Created by ferrum on 21/11/18.
//

#include "tcp_connection.h"


std::string make_response() {
    return "hello world";
}

tcp_connection::pointer tcp_connection::create(asio::io_context& io_context) {
    return pointer(new tcp_connection(io_context));
}

tcp::socket& tcp_connection::socket() {
    return socket_;
}

void tcp_connection::start() {

    beast::error_code error_code;
    beast::multi_buffer multi_buffer;

    http::request<http::string_body> request;

    http::read(socket_, multi_buffer, request, error_code);;

    std::cout << "REQUEST: " << request << std::endl;
    std::cout << "HEADERS: " << request.base() << std::endl;
    std::cout << "BODY: " << request.body() << std::endl;

    message_ = make_response();

    socket_.write_some(asio::buffer(message_));

}

template<
        class Body, class Allocator,
        class Send>
void
handle_request(
        beast::string_view doc_root,
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
    response.content_length(size);
    response.keep_alive(request.keep_alive());
    return send(std::move(response));
}


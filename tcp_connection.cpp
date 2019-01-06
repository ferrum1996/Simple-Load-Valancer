//
// Created by ferrum on 21/11/18.
//

#include "tcp_connection.h"




tcp_connection::pointer
tcp_connection::create(asio::io_context& io_context) {
    return pointer(new tcp_connection(io_context));
}

tcp::socket&
tcp_connection::socket() {
    return socket_;
}

void
tcp_connection::start() {

    beast::error_code error_code;
    beast::multi_buffer multi_buffer;
    bool close = false;
    http::request<http::string_body> request;

    http::read(socket_, multi_buffer, request, error_code);

    std::cout << "HEADERS: " << request.base() << std::endl;

    std::cout << "BODY: " << request.body() << std::endl;

    std::cout << "REMOTE ADDRESS: " << socket_.remote_endpoint().address().to_string() << std::endl;

    send_lambda<tcp::socket> lambda{socket_, close, error_code};

    handle_request(std::move(request), lambda);

}




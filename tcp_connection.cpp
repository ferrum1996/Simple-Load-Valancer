//
// Created by ferrum on 21/11/18.
//

#include "tcp_connection.h"

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


tcp_connection::pointer tcp_connection::create(asio::io_context& io_context) {
    return pointer(new tcp_connection(io_context));
}

tcp::socket& tcp_connection::socket() {
    return socket_;
}

void tcp_connection::start() {

    beast::error_code error_code;
    beast::multi_buffer multi_buffer;
    bool close = false;
    http::request<http::string_body> request;

    http::read(socket_, multi_buffer, request, error_code);;

    std::cout << "REQUEST: " << request << std::endl;
    std::cout << "HEADERS: " << request.base() << std::endl;
    std::cout << "BODY: " << request.body() << std::endl;

    send_lambda<tcp::socket> lambda{socket_, close, error_code};

    handle_request(std::move(request), lambda);

}




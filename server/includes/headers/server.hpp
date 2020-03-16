#ifndef SEND_ANYTHING_SERVER_HPP
#define SEND_ANYTHING_SERVER_HPP

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> asio_server;


namespace server {
    class Server {
    protected:
        asio_server m_server;

    public:
        explicit Server(int _port);

        int port;
    };

}

#endif //SEND_ANYTHING_SERVER_HPP

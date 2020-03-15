#ifndef SEND_ANYTHING_SERVER_HPP
#define SEND_ANYTHING_SERVER_HPP

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> asio_server;


namespace server {
    class Server {
    private:
        asio_server m_server;
        int port;
        void on_open(websocketpp::connection_hdl hdl);
        void on_message(websocketpp::connection_hdl hdl, const asio_server::message_ptr& msg);
    public:
        void start();
        explicit Server(int _port);
    };

}

#endif //SEND_ANYTHING_SERVER_HPP

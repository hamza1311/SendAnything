#ifndef SEND_ANYTHING_RECEIVING_SERVER_H
#define SEND_ANYTHING_RECEIVING_SERVER_H

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <string>

typedef websocketpp::server<websocketpp::config::asio> asio_server;


namespace server {
    class ReceivingServer {
    public:
        ReceivingServer(int _port, std::string path);

        void start();

        std::string saveDirectory{};
        int port;
    protected:
        void onMessage(const websocketpp::connection_hdl &hdl, const asio_server::message_ptr &msg);

        asio_server m_server;
    };
}

#endif //SEND_ANYTHING_RECEIVING_SERVER_H

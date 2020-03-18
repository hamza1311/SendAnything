#ifndef SEND_ANYTHING_SENDING_SERVER_H
#define SEND_ANYTHING_SENDING_SERVER_H

#include "headers/server.hpp"
#include <functional>

namespace server {

    class SendingServer {
    protected:
        void onOpen(websocketpp::connection_hdl hdl);

        void onMessage(websocketpp::connection_hdl hdl, const asio_server::message_ptr &msg);

        std::function<void ()> doneCallback;

        asio_server m_server;

    public:
        SendingServer(int port, const std::string &fileToSend, void _doneCallback());

        void start();

        std::string filepath;

        int port;

    };
}

#endif //SEND_ANYTHING_SENDING_SERVER_H

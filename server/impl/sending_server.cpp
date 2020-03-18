#include "headers/sending_server.h"
#include <iostream>
#include <fstream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using server::SendingServer;

void SendingServer::onMessage(websocketpp::connection_hdl hdl, const asio_server::message_ptr &msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::close || msg->get_payload() == "stfu") {
        m_server.stop(); // Forcefully stop the server because yes
        doneCallback();
    }
}

void SendingServer::onOpen(websocketpp::connection_hdl hdl) {
    std::cout << "Connection opened\n";

    std::ifstream ifs(filepath, std::ios::binary);
    std::string payload = std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    ifs.close();

    m_server.send(std::move(hdl), payload, websocketpp::frame::opcode::binary);
}

void SendingServer::start() {
    try {
        m_server.set_message_handler(bind(&SendingServer::onMessage, this, ::_1, ::_2));
        m_server.set_open_handler(bind(&SendingServer::onOpen, this, ::_1));
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

server::SendingServer::SendingServer(int _port, const std::string &fileToSend, void _doneCallback()) {
    port = _port;
    filepath = fileToSend;
    doneCallback = _doneCallback;

    m_server.init_asio();
}

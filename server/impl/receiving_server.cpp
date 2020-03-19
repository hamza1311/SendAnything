#include "headers/receiving_server.h"
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <ctime>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using server::ReceivingServer;

ReceivingServer::ReceivingServer(int _port, std::string path) {
    this->port = _port;
    this->saveDirectory = std::move(path);

    m_server.init_asio();
}

void ReceivingServer::start() {
    try {
        m_server.set_message_handler(bind(&ReceivingServer::onMessage, this, ::_1, ::_2));
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

void ReceivingServer::onMessage(const websocketpp::connection_hdl &hdl, const asio_server::message_ptr &msg) {
    try {
        std::time_t currentTime = std::time(nullptr);

        std::string filename = this->saveDirectory + "/" + std::to_string(currentTime);
        std::string payload = msg->get_payload();
        websocketpp::frame::opcode::value opcode = msg->get_opcode();


        if (opcode == websocketpp::frame::opcode::binary) {
            std::cout << filename << "\n";
            std::cout << "Size " << payload.size() << "\n";
            std::cout << "opcode " << opcode << "\n";

            std::ofstream outfile;
            outfile.open(filename);
            outfile << payload;
            outfile.close();

            std::cout << "\nBinary received\n";

            m_server.send(hdl, "Successfully received", websocketpp::frame::opcode::text);
            m_server.stop();
        }

    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    }
}
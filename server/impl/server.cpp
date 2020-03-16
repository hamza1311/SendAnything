#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

typedef websocketpp::server<websocketpp::config::asio> asio_server;

#include "headers/server.hpp"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using server::Server;

Server::Server(int _port) {
    this->port = _port;

    m_server.set_access_channels(websocketpp::log::alevel::none);
    m_server.clear_access_channels(websocketpp::log::alevel::none);
    m_server.init_asio();

//    m_server.set_message_handler(bind(&Server::onMessage, this, ::_1, ::_2));
}

/*
void Server::onMessage(websocketpp::connection_hdl hdl, const asio_server::message_ptr &msg) {
    try {
        std::string filename = "name.png"; // temp
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

            m_server.send(std::move(hdl), "Successfully received", websocketpp::frame::opcode::text);
        }

    } catch (websocketpp::exception const &e) {
        std::cout << e.what() << std::endl;
    }
}*/

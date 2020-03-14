#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <iostream>
#include <fstream>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server *s, websocketpp::connection_hdl hdl, const message_ptr& msg) {

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

            s->send(hdl, payload, msg->get_opcode());
        }

    } catch (websocketpp::exception const &e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

// Define a callback to handle connection opens
void on_open(server *s, websocketpp::connection_hdl hdl) {
    s->send(std::move(hdl), "Text", websocketpp::frame::opcode::binary);
    std::cout << "yes";
}

int main() {
    std::cout << "Hello World" << std::endl;
    // Create a server endpoint
    server server;

    try {
        // Set logging settings
        server.set_access_channels(websocketpp::log::alevel::none);
        server.clear_access_channels(websocketpp::log::alevel::none);

        // Initialize Asio
        server.init_asio();

        // Register our message handler
        server.set_message_handler(bind(&on_message, &server, ::_1, ::_2));
        server.set_open_handler(bind(&on_open, &server, ::_1));

        // Listen on port 9002
        server.listen(9002);

        // Start the server accept loop
        server.start_accept();

        // Start the ASIO io_service run loop
        server.run();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }

    return 0;
}

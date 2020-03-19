#include "headers/ui.hpp"
#include <thread>
#include "headers/receiving_server.h"
#include "headers/sending_server.h"

using ui::Handlers;

const int PORT = 9002;

void Handlers::onSendButtonClicked(Gtk::Window *window) {
    std::string path = ui::Utils::getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_OPEN, window);
    if (!path.empty()) {
        std::thread thread([path] {
            server::SendingServer server(PORT, path);
            server.start();
        });
        thread.detach();
    }
}

void Handlers::onReceiveButtonClicked(Gtk::Window *window) {
    std::string path = ui::Utils::getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER, window);
    if (!path.empty()) {
        std::thread thread([path] {
            server::ReceivingServer server(PORT, path);
            server.start();
        });
        thread.detach();
    }
}
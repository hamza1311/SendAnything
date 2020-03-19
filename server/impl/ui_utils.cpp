#include <iostream>
#include <thread>

#include "headers/receiving_server.h"
#include "headers/sending_server.h"
#include "headers/ui.hpp"

using ui::Utils;

const int PORT = 9002;

void Utils::onSendButtonClicked(Gtk::Window *window) {
    std::string path = Utils::getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_OPEN, window);
    if (!path.empty()) {
        std::thread thread([path] {
            server::SendingServer server(PORT, path);
            server.start();
        });
        thread.detach();
    }
}

void Utils::onReceiveButtonClicked(Gtk::Window *window) {
    std::string path = Utils::getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER, window);
    if (!path.empty()) {
        std::thread thread([path] {
            server::ReceivingServer server(PORT, path);
            server.start();
        });
        thread.detach();
    }
}

std::string Utils::getFileOrFolder(Gtk::FileChooserAction action, Gtk::Window *window) {
    std::cout << "Receive Clicked \n";

    Gtk::FileChooserDialog dialog("Please choose a file", action);
    dialog.set_transient_for(*window);

    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Open", Gtk::RESPONSE_OK);

    int result = dialog.run();
    std::string filename;
    if (result == Gtk::RESPONSE_OK) {
        filename = dialog.get_filename();
        std::cout << "File selected: " << filename << std::endl;
    }

    return filename;
}

Gtk::Button *Utils::loadButton(
        const Glib::RefPtr<Gtk::Builder> &builder,
        const std::string &name, void handler(Gtk::Window *),
        Gtk::Window *window
) {
    Gtk::Button *button = nullptr;
    builder->get_widget(name, button);
    button->signal_clicked().connect(sigc::bind(handler, window));
    return button;
}

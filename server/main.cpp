#include "headers/sending_server.h"
#include <gtkmm.h>
#include <iostream>
#include <thread>
#include <functional>
#include <headers/receiving_server.h>

Gtk::Window *window = nullptr;

std::string getFileOrFolder(Gtk::FileChooserAction action) {
    std::cout << "Receive Clicked \n";

    Gtk::FileChooserDialog dialog("Please choose a file", action);
    dialog.set_transient_for(*window);

    //Add response buttons the the dialog:
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Open", Gtk::RESPONSE_OK);

    int result = dialog.run();
    std::string filename;
    switch (result) {
        case (Gtk::RESPONSE_OK): {
            std::cout << "Open clicked." << std::endl;

            filename = dialog.get_filename();
            std::cout << "File selected: " << filename << std::endl;
            break;
        }
        case (Gtk::RESPONSE_CANCEL): {
            std::cout << "Cancel clicked." << std::endl;
            break;
        }
        default: {
            std::cout << "Unexpected button clicked." << std::endl;
            break;
        }
    }
    return filename;
}

class Task {
public:
    static void send(std::string path) {
        server::SendingServer server(9002, path);
        server.start();
    }

    static void receive(std::string path) {
        server::ReceivingServer server(9002, path);
        server.start();
    }
};


void onSendButtonClicked() {

    std::cout << "Send Clicked \n";

    std::string path = getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_OPEN);
    if (!path.empty()) {
        std::thread thread(&Task::send, path);
        thread.detach();
    }
}

void onReceiveButtonClicked() {
    std::cout << "Receive Clicked\n";

    std::string path = getFileOrFolder(Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    if (!path.empty()) {
        std::thread thread(&Task::receive, path);
        thread.detach();
    }
}

Gtk::Button *loadButton(const Glib::RefPtr<Gtk::Builder> &builder, const std::string &name, void handler()) {
    Gtk::Button *button = nullptr;
    builder->get_widget(name, button);
    button->signal_clicked().connect(sigc::ptr_fun(handler));
    return button;
}

int main(int argc, char *argv[]) {

    auto app = Gtk::Application::create(argc, argv, "dev.hamza.send-anything");

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
    try {
        builder->add_from_file("ui/MainWindowUI.glade");
    } catch (const Glib::FileError &ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch (const Glib::MarkupError &ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Gtk::BuilderError &ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    builder->get_widget("MainWindow", window);

    loadButton(builder, "SendButton", onSendButtonClicked);
    loadButton(builder, "ReceiveButton", onReceiveButtonClicked);

    return app->run(*window);
}

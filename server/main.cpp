#include "headers/sending_server.h"
#include <gtkmm.h>
#include <iostream>

Gtk::Window *window = nullptr;

void onSendButtonClicked() {

    std::cout << "Send Clicked \n";
    // TODO: Load file picker, get file path
    std::string path = "/home/hamza/Pictures/385149.jpg"; // Get this path from user
    server::SendingServer server(9002, path, [] { window->close(); });
    server.start();

}

void onReceiveButtonClicked() {
    std::cout << "Receive Clicked \n";
    // TODO: Load file picker, get save location, start server send file
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

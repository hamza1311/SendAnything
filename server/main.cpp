#include <gtkmm.h>
#include <iostream>
#include "headers/ui.hpp"

int main(int argc, char *argv[]) {

    auto app = Gtk::Application::create(argc, argv, "dev.hamza.send-anything");
    Gtk::Window *window = nullptr;

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

    ui::Utils::loadButton(builder, "SendButton", ui::Utils::onSendButtonClicked, window);
    ui::Utils::loadButton(builder, "ReceiveButton", ui::Utils::onReceiveButtonClicked, window);

    return app->run(*window);
}

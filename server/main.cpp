//#include "headers/server.hpp"
#include <gtkmm.h>

int main(int argc, char *argv[]) {
//    server::Server server(9002);
//    server.start();

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
    Gtk::Window window;
    window.set_default_size(200, 200);

    return app->run(window);
}

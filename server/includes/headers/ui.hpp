#ifndef SEND_ANYTHING_UI_HPP
#define SEND_ANYTHING_UI_HPP

#include <gtkmm.h>

namespace ui {
    class Utils {
    public:

        static std::string getFileOrFolder(Gtk::FileChooserAction action, Gtk::Window *window);

        static Gtk::Button *loadButton(
                const Glib::RefPtr<Gtk::Builder> &builder,
                const std::string &name, void handler(Gtk::Window *),
                Gtk::Window *window
        );
    };

    class Handlers {
    public:
        static void onSendButtonClicked(Gtk::Window *window);

        static void onReceiveButtonClicked(Gtk::Window *window);
    };

}

#endif //SEND_ANYTHING_UI_HPP

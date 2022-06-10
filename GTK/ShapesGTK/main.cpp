#include <gtkmm.h>
#include "Window.h"

int main(int argc, char *argv[]) {
    auto Application = Gtk::Application::create(argc, argv);
    MyWindow window;
    window.set_position(Gtk::WIN_POS_CENTER);
    return Application->run(window);
}

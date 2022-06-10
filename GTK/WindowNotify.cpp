#include <gtkmm.h>
#include <string>
void showInfoDialog(std::string secondaryMessage) {
    Gtk::MessageDialog dialog("Информация", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_CANCEL);
    dialog.set_secondary_text(secondaryMessage);
    dialog.set_default_response(Gtk::RESPONSE_YES);
    dialog.set_position(Gtk::WIN_POS_CENTER);
    dialog.run();
}
#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowBackup::WindowBackup(App *app) {
    this->app = app;
    this->menu_window = newwin(24, 80, 1, 0);
    keypad(this->menu_window, true);
    this->path = getenv("HOME");
    TBTUI::Util::get_dir(this->path, this->listing);
    this->items = new ITEM*[listing.size() + 1];
    ITEM **item_iter = this->items;
    for (const auto &i : listing) {
        *item_iter = new_item(i.c_str(), "");
        ++item_iter;
    }
    *item_iter = NULL;
    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, derwin(this->menu_window, 23, 80, 1, 0));
    post_menu(this->menu);
}

TBTUI::App::WindowBackup::~WindowBackup() {
    unpost_menu(this->menu);
    free_menu(this->menu);
    free_item(this->items[0]);
    free_item(this->items[1]);
    delete[] this->items;
    delwin(this->menu_window);
}

void TBTUI::App::WindowBackup::render() {
    clear();
    mvprintw(0, 0, "TBTUI - Backup");
    refresh();
    mvwprintw(this->menu_window, 0, 0, this->path.c_str());
    wrefresh(this->menu_window);
}

TBTUI::App::Window::HandlerStatus TBTUI::App::WindowBackup::handle() {
    bool exit = false;
    bool render = false;
    int in = wgetch(this->menu_window);
    switch (in) {
    case 'Q':
    case 'q':
        exit = true;
        break;
    case KEY_UP:
        menu_driver(this->menu, REQ_UP_ITEM);
        wrefresh(this->menu_window);
        break;
    case KEY_DOWN:
        menu_driver(this->menu, REQ_DOWN_ITEM);
        wrefresh(this->menu_window);
        break;
    case KEY_BACKSPACE:
        render = true;
        this->app->windows.pop_back();
        break;
    case KEY_ENTER:
        break;
    }
    return HandlerStatus(exit, render);
}

#include "../app.h"

tbtui::App::WindowBackup::WindowBackup(App *app) {
    this->app = app;
    this->menu_window = newwin(24, 80, 1, 0);
    keypad(this->menu_window, true);
    this->items = new ITEM*[24];
    this->items[0] = new_item("XXX", "");
    this->items[1] = new_item("XXXA", "");
    this->items[2] = NULL;
    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, derwin(this->menu_window, 2, 10, 1, 0));
    post_menu(this->menu);
}

tbtui::App::WindowBackup::~WindowBackup() {
    unpost_menu(this->menu);
    free_menu(this->menu);
    free_item(this->items[0]);
    free_item(this->items[1]);
    delete[] this->items;
    delwin(this->menu_window);
}

void tbtui::App::WindowBackup::render() {
    clear();
    mvprintw(0, 0, "TBTUI - Backup");
    refresh();
    wrefresh(this->menu_window);
}

tbtui::App::Window::HandlerStatus tbtui::App::WindowBackup::handle() {
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

#include "../app.h"

TBTUI::App::WindowMain::WindowMain(App *app) {
    this->app = app;
    this->menu_window = newwin(2, 10, 1, 0);
    keypad(this->menu_window, true);
    this->items[0] = new_item("Backup", "");
    this->items[1] = new_item("Restore", "");
    this->items[2] = NULL;
    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, derwin(this->menu_window, 2, 10, 0, 0));
    post_menu(this->menu);
}

TBTUI::App::WindowMain::~WindowMain() {
    unpost_menu(this->menu);
    free_menu(this->menu);
    free_item(this->items[0]);
    free_item(this->items[1]);
    delwin(this->menu_window);
}

void TBTUI::App::WindowMain::render() {
    clear();
    mvprintw(0, 0, "TBTUI");
    refresh();
    wrefresh(this->menu_window);
}

TBTUI::App::Window::HandlerStatus TBTUI::App::WindowMain::handle() {
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
    case '\n':
        switch (item_index(current_item(this->menu))) {
        case 0:
            render = true;
            this->app->windows.push_back(
                std::make_unique<WindowBackup>(this->app)
            );
            break;
        case 1:
            break;
        }
    }
    return HandlerStatus(exit, render);
}

#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowBackupRun::WindowBackupRun(App *app, std::string path) {
    this->state = State::INITIALIZATION;
    this->app = app;
    bool is_valid = false;
    TBTUI::Util::check_dev(this->app->dev_name, this->signature);
    is_valid = this->app->conn.check(this->signature);
    this->console_window = newwin(24, 40, 1, 40);
    scrollok(this->console_window, true);
    if (is_valid) {
        wprintw(this->console_window, "Found valid signature\n");
    } else {
        waddstr(this->console_window, "Creating new\nWrite protect?\n");
        this->items = new ITEM*[3];
        this->items[0] = new_item("Yes", "");
        this->items[1] = new_item("No", "");
        this->items[2] = NULL;
        this->state = State::WRITE_PROTECT;
    }
    this->menu_window = newwin(24, 40, 1, 0);
    keypad(this->menu_window, true);
    this->path = path;

    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, derwin(this->menu_window, 23, 40, 1, 0));
    post_menu(this->menu);
}

TBTUI::App::WindowBackupRun::~WindowBackupRun() {
    unpost_menu(this->menu);
    free_menu(this->menu);
    for (ITEM **item_iter = this->items; *item_iter; ++item_iter) {
        free_item(*item_iter);
    }
    delete[] this->items;
    delwin(this->menu_window);
    delwin(this->console_window);
}

void TBTUI::App::WindowBackupRun::render() {
    clear();
    mvprintw(0, 0, "TBTUI - Backup - Run");
    refresh();
    mvwprintw(this->menu_window, 0, 0, "Backing up %s", this->path.c_str());
    wrefresh(this->menu_window);
    wrefresh(this->console_window);
}

TBTUI::App::Window::HandlerStatus TBTUI::App::WindowBackupRun::handle() {
    bool exit = false;
    bool render = false;
    int in = wgetch(this->menu_window);
    switch (in) {
    case 'Q':
    case 'q':
        exit = true;
        break;
    case
    }
    return HandlerStatus(exit, render);
}

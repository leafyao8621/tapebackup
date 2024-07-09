#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowBackupRun::WindowBackupRun(App *app, std::string path) {
    this->state = State::INITIALIZATION;
    this->write_protect = false;
    this->app = app;
    bool is_valid = false;
    this->console_window = newwin(24, 40, 1, 40);
    scrollok(this->console_window, true);
    TBTUI::Util::check_dev(this->app->dev_name, this->signature);
    is_valid = this->app->conn.check(this->signature);
    if (is_valid) {
        Util::get_hex(this->signature, this->buf);
        wprintw(this->console_window, "Found valid signature\n%s\n", this->buf);
        this->items = new ITEM*[1];
        this->items[0] = NULL;
    } else {
        waddstr(this->console_window, "Generate new signature?\n");
        this->items = new ITEM*[3];
        this->items[0] = new_item("Yes", "");
        this->items[1] = new_item("No", "");
        this->items[2] = NULL;
        this->state = State::GENERATE_SIGNATURE;
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
    mvwprintw(
        this->menu_window, 0, 0,
        "Backing up %s",
        this->path.size() > 29 ?
        this->path.substr(this->path.size() - 29, 29).c_str() :
        this->path.c_str()
    );
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
    case KEY_UP:
        menu_driver(this->menu, REQ_UP_ITEM);
        wrefresh(this->menu_window);
        break;
    case KEY_DOWN:
        menu_driver(this->menu, REQ_DOWN_ITEM);
        wrefresh(this->menu_window);
        break;
    case '\n':
        switch (this->state) {
        case INITIALIZATION:
            break;
        case GENERATE_SIGNATURE:
            if (item_index(current_item(this->menu))) {
                render = true;
                this->app->windows.pop_back();
                return HandlerStatus(exit, render);
            }
            this->app->gen(this->signature);
            Util::get_hex(this->signature, this->buf);
            wprintw(
                this->console_window,
                "Generated signature\n%s\nWrite Protect?\n",
                this->buf
            );
            wrefresh(this->console_window);
            this->state = WRITE_PROTECT;
            break;
        case WRITE_PROTECT:
            this->write_protect = !item_index(current_item(this->menu));
            Util::init_dev(
                this->app->dev_name,
                this->signature,
                this->write_protect
            );
            this->app->conn.add(this->signature, this->write_protect);
            wprintw(
                this->console_window,
                "Tape initialized\nInitiate archiving?"
            );
            wrefresh(this->console_window);
            this->state = INITIATE_ARCHIVING;
            break;
        case INITIATE_ARCHIVING:
            if (item_index(current_item(this->menu))) {
                render = true;
                this->app->windows.pop_back();
                return HandlerStatus(exit, render);
            }

            break;
        }
        break;
    }
    return HandlerStatus(exit, render);
}

#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowBackup::WindowBackup(App *app) {
    this->app = app;
    this->menu_window = newwin(24, 80, 1, 0);
    this->menu_window_sub = derwin(this->menu_window, 23, 80, 1, 0);
    keypad(this->menu_window, true);
    this->path = getenv("HOME");
    TBTUI::Util::get_dir(this->path, this->listing);
    this->items = new ITEM*[listing.size() + 1];
    ITEM **item_iter = this->items;
    for (const auto &i : this->listing) {
        *item_iter = new_item(i.c_str(), "");
        ++item_iter;
    }
    *item_iter = NULL;
    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, this->menu_window_sub);
    post_menu(this->menu);
    this->pos = 0;
}

TBTUI::App::WindowBackup::~WindowBackup() {
    unpost_menu(this->menu);
    free_menu(this->menu);
    for (ITEM **item_iter = this->items; *item_iter; ++item_iter) {
        free_item(*item_iter);
    }
    delete[] this->items;
    delwin(this->menu_window);
}

void TBTUI::App::WindowBackup::render() {
    clear();
    mvprintw(0, 0, "TBTUI - Backup");
    refresh();
    mvwaddstr(this->menu_window, 0, 0, this->path.c_str());
    wrefresh(this->menu_window);
}

TBTUI::App::Window::HandlerStatus TBTUI::App::WindowBackup::handle() {
    bool exit = false;
    bool render = false;
    int in = wgetch(this->menu_window);
    ITEM **item_iter = nullptr;
    size_t slash = 0;
    std::string dir = "";
    switch (in) {
    case 'Q':
    case 'q':
        exit = true;
        break;
    case KEY_UP:
        if (!this->pos) {
            menu_driver(this->menu, REQ_SCR_UPAGE);
        } else {
            --this->pos;
        }
        menu_driver(this->menu, REQ_UP_ITEM);
        wrefresh(this->menu_window);
        break;
    case KEY_DOWN:
        if (this->pos == 22) {
            menu_driver(this->menu, REQ_SCR_DPAGE);
        } else {
            if (this->pos < this->listing.size() - 1) {
                ++this->pos;
            }
        }
        menu_driver(this->menu, REQ_DOWN_ITEM);
        wrefresh(this->menu_window);
        break;
    case KEY_BACKSPACE:
        render = true;
        this->app->windows.pop_back();
        break;
    case KEY_NPAGE:
        if (!this->listing.size()) {
            break;
        }
        this->path += '/';
        this->path += current_item(this->menu)->name.str;
        try {
            TBTUI::Util::get_dir(this->path, this->listing);
        } catch (TBTUI::Util::Err err) {
            slash = this->path.find_last_of('/');
            this->path.erase(this->path.begin() + slash, this->path.end());
            TBTUI::Util::get_dir(this->path, this->listing);
            break;
        }
        unpost_menu(this->menu);
        free_menu(this->menu);
        for (item_iter = this->items; *item_iter; ++item_iter) {
            free_item(*item_iter);
        }
        delete[] this->items;
        this->items = new ITEM*[listing.size() + 1];
        item_iter = this->items;
        for (const auto &i : this->listing) {
            *item_iter = new_item(i.c_str(), "");
            ++item_iter;
        }
        *item_iter = NULL;
        this->menu = new_menu(this->items);
        set_menu_win(this->menu, this->menu_window);
        set_menu_sub(this->menu, this->menu_window_sub);
        post_menu(this->menu);
        this->pos = 0;
        render = true;
        break;
    case KEY_PPAGE:
        slash = this->path.find_last_of('/');
        dir = this->path.substr(slash, this->path.size() - slash);
        this->path.erase(this->path.begin() + slash, this->path.end());
        try {
            TBTUI::Util::get_dir(this->path, this->listing);
        } catch (TBTUI::Util::Err err) {
            this->path += dir;
            TBTUI::Util::get_dir(this->path, this->listing);
            break;
        }
        unpost_menu(this->menu);
        free_menu(this->menu);
        for (item_iter = this->items; *item_iter; ++item_iter) {
            free_item(*item_iter);
        }
        delete[] this->items;
        this->items = new ITEM*[listing.size() + 1];
        item_iter = this->items;
        for (const auto &i : this->listing) {
            *item_iter = new_item(i.c_str(), "");
            ++item_iter;
        }
        *item_iter = NULL;
        this->menu = new_menu(this->items);
        set_menu_win(this->menu, this->menu_window);
        set_menu_sub(this->menu, this->menu_window_sub);
        post_menu(this->menu);
        this->pos = 0;
        render = true;
        break;
    case KEY_ENTER:
        break;
    }
    return HandlerStatus(exit, render);
}

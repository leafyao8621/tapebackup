#include <pthread.h>

#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowBackupRun::WindowBackupRun(App *app, std::string path) {
    this->state = State::INITIALIZATION;
    this->running[0] = false;
    this->running[1] = false;
    this->cleanup[0] = false;
    this->cleanup[1] = false;
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
        bool write_protection_dev =
            Util::check_dev_write_protection(this->app->dev_name);
        bool write_protection_db =
            this->app->conn.get_write_protection(this->signature);
        if (write_protection_db != write_protection_dev) {
            wprintw(this->console_window, "%s\n", "Write protect tampered");
            this->items = new ITEM*[2];
            this->items[0] = new_item("Yes", "");
            this->items[1] = NULL;
            this->state = ERROR;
        } else if (write_protection_db) {
            wprintw(this->console_window, "%s\n", "Write protected");
            this->items = new ITEM*[2];
            this->items[0] = new_item("Yes", "");
            this->items[1] = NULL;
            this->state = ERROR;
        } else {
            this->items = new ITEM*[3];
            wprintw(this->console_window, "%s\n", "Write protect?");
            this->items[0] = new_item("Yes", "");
            this->items[1] = new_item("No", "");
            this->items[2] = NULL;
            this->state = State::SET_WRITE_PROTECT;
        }
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
    for (size_t i = 0; i < 2; ++i) {
        if (this->running[i]) {
            pthread_cancel(this->pool[i].native_handle());
        }
        if (this->cleanup[i]) {
            this->pool[i].join();
        }
    }
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
    case KEY_BACKSPACE:
        render = true;
        this->app->windows.pop_back();
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
                "%s\n",
                "Tape initialized\nInitiate archiving?"
            );
            wrefresh(this->console_window);
            this->state = INITIATE_ARCHIVING;
            break;
        case SET_WRITE_PROTECT:
            this->write_protect = !item_index(current_item(this->menu));
            if (this->write_protect) {
                Util::set_dev_write_protection(this->app->dev_name);
                this->app->conn.set_write_protection(this->signature);
                wprintw(
                    this->console_window,
                    "%s\n",
                    "Write protect set"
                );
            }
            wprintw(
                this->console_window,
                "%s\n",
                "Initiate archiving?"
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
            this->pool[0] =
                std::thread(
                    [](
                        std::string path,
                        bool *cleanup,
                        bool *running,
                        State *state,
                        WINDOW *window) {
                        *cleanup = true;
                        *running = true;
                        *state = State::ARCHIVING;
                        wprintw(window, "Archiving\n");
                        wrefresh(window);
                        Util::Archiver archiver;
                        archiver((char*)path.c_str());
                        wprintw(
                            window, "%s",
                            "Archiving completed\n"
                            "Hit enter to go back after completion\n"
                            "Initiate writing?\n"
                        );
                        wrefresh(window);
                        *state = State::INITIATE_WRITING;
                        *running = false;
                    },
                    this->path,
                    &this->cleanup[0],
                    &this->running[0],
                    &this->state,
                    this->console_window
                );
            break;
        case ARCHIVING:
            break;
        case INITIATE_WRITING:
            if (item_index(current_item(this->menu))) {
                render = true;
                this->app->windows.pop_back();
                return HandlerStatus(exit, render);
            }
            this->app->gen(this->hmac_key);
            this->app->conn.update_key(this->signature, this->hmac_key);
            Util::get_hex(this->hmac_key, this->buf);
            wprintw(
                this->console_window,
                "Generated HMAC key\n%s\n",
                this->buf
            );
            wrefresh(this->console_window);
            unpost_menu(this->menu);
            free_menu(this->menu);
            free_item(this->items[1]);
            this->items[1] = NULL;
            this->menu = new_menu(this->items);
            set_menu_win(this->menu, this->menu_window);
            set_menu_sub(this->menu, derwin(this->menu_window, 23, 40, 1, 0));
            post_menu(this->menu);
            wrefresh(this->menu_window);
            this->state = WRITING;
            this->pool[0].join();
            this->pool[0] =
                std::thread(
                    [](
                        std::string path,
                        char *dev,
                        size_t block_size,
                        bool *cleanup,
                        bool *running,
                        WINDOW *window,
                        std::mutex &mutex) {
                        *cleanup = true;
                        *running = true;
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(window, "Writing\n");
                            wrefresh(window);
                        }
                        Util::Writer writer(block_size);
                        writer((char*)path.c_str(), dev, window, mutex);
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(window, "Writing completed\n");
                            wrefresh(window);
                        }
                        *running = false;
                    },
                    getenv("HOME") + std::string("/backup/backup.tar"),
                    this->app->dev_name,
                    this->app->block_size_write,
                    &this->cleanup[0],
                    &this->running[0],
                    this->console_window,
                    std::ref(this->mutex)
                );
            this->pool[1] =
                std::thread(
                    [](
                        std::string path,
                        char *hmac_key,
                        char *hmac_md,
                        size_t block_size,
                        bool *cleanup,
                        bool *running,
                        WINDOW *window,
                        std::mutex &mutex) {
                        *cleanup = true;
                        *running = true;
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(window, "Calculating\n");
                            wrefresh(window);
                        }
                        Util::HMAC hmac(block_size);
                        hmac(
                            (char*)path.c_str(),
                            hmac_key,
                            hmac_md,
                            false,
                            window,
                            mutex
                        );
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(window, "Calculating completed\n");
                            wrefresh(window);
                        }
                        wrefresh(window);
                        *running = false;
                    },
                    getenv("HOME") + std::string("/backup/backup.tar"),
                    this->hmac_key,
                    this->hmac_md,
                    this->app->block_size_hmac,
                    &this->cleanup[1],
                    &this->running[1],
                    this->console_window,
                    std::ref(this->mutex)
                );
            break;
        case WRITING:
            if (this->running[0] || this->running[1]) {
                break;
            }
            this->app->conn.update_hmac(this->signature, this->hmac_md);
            render = true;
            this->app->windows.pop_back();
            break;
        case ERROR:
            render = true;
            this->app->windows.pop_back();
            break;
        }
    }
    return HandlerStatus(exit, render);
}

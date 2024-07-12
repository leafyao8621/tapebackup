#include <cstring>

#include <pthread.h>

#include "../app.h"
#include "../../util/util.h"

TBTUI::App::WindowCheck::WindowCheck(App *app) {
    this->state = INITIALIZATION;
    this->running = false;
    this->cleanup = false;
    this->app = app;
    this->console_window = newwin(24, 40, 1, 40);
    scrollok(this->console_window, true);
    this->menu_window = newwin(24, 40, 1, 0);
    keypad(this->menu_window, true);
    this->items[0] = new_item("Yes", "");
    this->items[1] = NULL;
    this->menu = new_menu(this->items);
    set_menu_win(this->menu, this->menu_window);
    set_menu_sub(this->menu, derwin(this->menu_window, 23, 40, 1, 0));
    post_menu(this->menu);
}

TBTUI::App::WindowCheck::~WindowCheck() {
    if (this->running) {
        pthread_cancel(this->worker.native_handle());
    }
    if (this->cleanup) {
        this->worker.join();
    }
    unpost_menu(this->menu);
    free_menu(this->menu);
    free_item(this->items[0]);
    delwin(this->menu_window);
    delwin(this->console_window);
}

void TBTUI::App::WindowCheck::render() {
    clear();
    mvprintw(0, 0, "TBTUI - Check");
    refresh();
    mvwprintw(
        this->menu_window, 0, 0,
        "%s",
        "Check?"
    );
    wrefresh(this->menu_window);
    wrefresh(this->console_window);
}

TBTUI::App::Window::HandlerStatus TBTUI::App::WindowCheck::handle() {
    bool exit = false;
    bool render = false;
    int in = wgetch(this->menu_window);
    bool dev_check = false;
    bool write_protect_dev = false;
    bool write_protect_db = false;
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
            Util::check_dev(this->app->dev_name, this->signature);
            dev_check = this->app->conn.check(this->signature);
            if (!dev_check) {
                wprintw(
                    this->console_window, "%s\n",
                    "No valid signature found"
                );
                wrefresh(this->console_window);
                this->state = ERROR;
                break;
            }
            Util::get_hex(this->signature, this->buf);
            wprintw(
                this->console_window, "Valid signature found\n%s\n",
                this->buf
            );
            wrefresh(this->console_window);
            write_protect_db =
                this->app->conn.get_write_protection(this->signature);
            write_protect_dev =
                Util::check_dev_write_protection(this->app->dev_name);
            if (write_protect_db != write_protect_dev) {
                wprintw(
                    this->console_window, "%s\n",
                    "Write protect tampered"
                );
                wrefresh(this->console_window);
                this->state = ERROR;
                break;
            }
            if (write_protect_db) {
                wprintw(
                    this->console_window, "%s\n",
                    "Write protect found"
                );
                wrefresh(this->console_window);
            }
            this->app->conn.get_key(this->signature, this->hmac_key);
            Util::get_hex(this->hmac_key, this->buf);
            wprintw(
                this->console_window, "HMAC Key\n%s\n",
                this->buf
            );
            wrefresh(this->console_window);
            this->app->conn.get_hmac(this->signature, this->hmac_md);
            Util::get_hex(this->hmac_md, this->buf);
            wprintw(
                this->console_window, "HMAC in DB\n%s\n",
                this->buf
            );
            wrefresh(this->console_window);
            this->state = CHECKING;
            this->worker =
                std::thread(
                    [](
                        char *path,
                        char *key,
                        char *md,
                        char *md_db,
                        WINDOW *window,
                        std::mutex &mutex,
                        bool *running,
                        bool *cleanup,
                        size_t block_size,
                        State *state,
                        char *buf) {
                        Util::HMAC hmac(block_size);
                        hmac(path, key, md, true, window, mutex);
                        if (memcmp(md, md_db, 64)) {
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(window, "%s\n", "File tampered");
                            wrefresh(window);
                            *state = ERROR;
                        } else {
                            Util::get_hex(md_db, buf);
                            std::lock_guard<std::mutex> guard(mutex);
                            wprintw(
                                window,
                                "Signature verified\n%s\n",
                                buf
                            );
                            wrefresh(window);
                        }
                        *running = false;
                        *cleanup = true;
                    },
                    this->app->dev_name,
                    this->hmac_key,
                    this->hmac_md_dev,
                    this->hmac_md,
                    this->console_window,
                    std::ref(this->mutex),
                    &this->running,
                    &this->cleanup,
                    this->app->block_size_hmac,
                    &this->state,
                    this->buf
                );
            break;
        case CHECKING:
            if (this->running) {
                break;
            }
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

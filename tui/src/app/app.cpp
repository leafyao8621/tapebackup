#include <csignal>
#include <ncurses.h>

#include "app.h"
#include "../util/util.h"

TBTUI::App::Window::HandlerStatus::HandlerStatus(bool exit, bool render) {
    this->exit = exit;
    this->render = render;
}

TBTUI::App::App(char *dev_name) {
    this->block_size_write = 65536;
    this->block_size_hmac = 65536;
    this->block_size_read = 65536;
    signal(SIGINT, SIG_IGN);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    this->dev_name = dev_name;
    try {
        TBTUI::Util::Env env;
        bool env_check = env.check();
        if (!env_check) {
            env.init();
        }
    } catch (TBTUI::Util::Env::Err err) {
        endwin();
        throw Err::FOLDER_CREATION;
    }
}

TBTUI::App::~App() {
    endwin();
}

void TBTUI::App::run() {
    this->windows.push_back(std::make_unique<WindowMain>(this));
    this->windows.back()->render();
    for (;;) {
        Window::HandlerStatus status = this->windows.back()->handle();
        if (status.exit) {
            break;
        }
        if (status.render) {
            this->windows.back()->render();
        }
    }
}

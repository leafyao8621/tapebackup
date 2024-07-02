#include <csignal>
#include <ncurses.h>

#include "app.h"

TBTUI::App::Window::HandlerStatus::HandlerStatus(bool exit, bool render) {
    this->exit = exit;
    this->render = render;
}

TBTUI::App::App() {
    signal(SIGINT, SIG_IGN);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    this->windows.push_back(std::make_unique<WindowMain>(this));
}

TBTUI::App::~App() {
    endwin();
}

void TBTUI::App::run() {
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

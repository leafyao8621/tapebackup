#ifndef APP_APP_H_
#define APP_APP_H_

#include <vector>
#include <memory>
#include <string>

#include <ncurses.h>
#include <menu.h>

namespace TBTUI {
    class App {
    public:
        class Window {
        protected:
            App *app;
        public:
            class HandlerStatus {
            public:
                bool exit;
                bool render;
                HandlerStatus(bool exit, bool render);
            };
            virtual ~Window() {};
            virtual HandlerStatus handle() = 0;
            virtual void render() = 0;
        };
        class WindowMain : public Window {
            MENU *menu;
            ITEM *items[3];
            WINDOW *menu_window, *menu_window_sub;
        public:
            WindowMain(App *app);
            ~WindowMain();
            HandlerStatus handle();
            void render();
        };
        class WindowBackup : public Window {
            MENU *menu;
            ITEM **items;
            WINDOW *menu_window, *menu_window_sub;
            std::string path;
            std::vector<std::string> listing;
            size_t pos;
        public:
            WindowBackup(App *app);
            ~WindowBackup();
            HandlerStatus handle();
            void render();
        };
        enum Err {
            INIT
        };
    private:
        friend class WindowMain;
        friend class WindowBackup;
        std::vector<std::unique_ptr<Window> > windows;
    public:
        App();
        ~App();
        void run();
    };
}

#endif

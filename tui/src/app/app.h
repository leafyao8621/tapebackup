#ifndef APP_APP_H_
#define APP_APP_H_

#include <vector>
#include <memory>
#include <string>

#include <ncurses.h>
#include <menu.h>

#include "../db/connector.h"
#include "../util/util.h"

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
            WINDOW *menu_window;
        public:
            WindowMain(App *app);
            ~WindowMain();
            HandlerStatus handle();
            void render();
        };
        class WindowBackup : public Window {
            MENU *menu;
            ITEM **items;
            WINDOW *menu_window;
            std::string path;
            std::vector<std::string> listing;
            size_t pos;
        public:
            WindowBackup(App *app);
            ~WindowBackup();
            HandlerStatus handle();
            void render();
        };
        class WindowBackupRun : public Window {
            enum State {
                INITIALIZATION,
                WRITE_PROTECT
            };
            State state;
            MENU *menu;
            ITEM **items;
            WINDOW *menu_window, *console_window;
            std::string path;
            char signature[64], buf[129];
        public:
            WindowBackupRun(App *app, std::string path);
            ~WindowBackupRun();
            HandlerStatus handle();
            void render();
        };
        enum Err {
            INIT,
            FOLDER_CREATION
        };
    private:
        friend class WindowMain;
        friend class WindowBackup;
        friend class WindowBackupRun;
        Connector conn;
        char *dev_name;
        Util::Gen gen;
        std::vector<std::unique_ptr<Window> > windows;
    public:
        App(char *dev_name);
        ~App();
        void run();
    };
}

#endif

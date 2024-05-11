#ifndef APP_APP_H_
#define APP_APP_H_

#include <cstdio>
#include <vector>

namespace TBCLI {
    class App {
    public:
        enum Err {
            DEVICE_OPEN,
            DEVICE_NOT_INTIALIZED,
            DEVICE_WRITE_PROTECTED
        };
        class Signature {
            std::vector<std::vector<char> > valid_list;
            int rnd_src, signature_file;
        public:
            enum Err {
                GENERATOR_FAIL,
                WRITE_FAIL
            };
            Signature();
            ~Signature();
            void init();
            void generate(char *signature) const;
            void commit(char *signature) const;
            bool check(char *signature) const;
        };
    private:
        int dev;
        Signature signature;
    public:
        App(char *dev_name);
        ~App();
        void main_loop() const;
    private:
        void read_dev() const;
        void write_dev() const;
    };
}

#endif

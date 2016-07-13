#ifndef _MOVETOFRONT_H_
#define _MOVETOFRONT_H_

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

namespace bw
{
    class MoveToFront {
        // apply move-to-front encoding, reading from standard input and writing to standard output
        private:
            class Item
            {
                private:
                    char value;
                public:
                    Item(char v) {
                        value = v;
                    }
                    char get() { return value; }
                    void set(char c) { value = c; }
                    void inc() { value++; }
            };

        public:
            void static encode(std::istream &streamin, std::ostream &streamout)
            {
                std::vector<std::shared_ptr<Item>> ct(256);
                std::vector<std::shared_ptr<Item>> cv(256);

                std::stringstream sb;

                for (int i = 0; i < 256; i++) {
                    ct[i] = std::shared_ptr<Item>(new Item(i));
                    cv[i] = ct[i];
                }

                char c;
                while (streamin.get(c)) {
                    std::shared_ptr<Item> pos = ct[c];
                    for (int i = pos->get(); i > 0; i--) {
                        cv[i] = cv[i-1];
                        cv[i]->inc();
                    }
                    cv[0] = pos;
                    sb.put(pos->get());
                    pos->set(0);
                }

                streamout << sb.rdbuf();
                streamout.flush();
            }

            // apply move-to-front decoding, reading from standard input and writing to standard output
            void static decode(std::istream &streamin, std::ostream &streamout)
            {
                char c, p;
                std::vector<char> cv(256);
                std::stringstream sb;

                for (int i = 0; i < 256; i++) {
                    cv[i] = i;
                }

                while (streamin.get(p)) {
                    c = cv[p];
                    sb.put(c);

                    for (int i = p; i > 0; i--) {
                        cv[i] = cv[i-1];
                    }
                    cv[0] = c;
                }
                streamout << sb.rdbuf();
                streamout.flush();
            }
    };
}
#endif

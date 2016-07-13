#ifndef _BURROWSWHEELER_H_
#define _BURROWSWHEELER_H_

#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <iostream>

#include "CircularSuffixArray.h"

namespace bw
{
    class BurrowsWheeler {
        // apply Burrows-Wheeler encoding, reading from standard input and writing to standard output
        private:
            class Item
            {
                private:
                    std::queue<int> q;
                    int count;

                public:
                    Item()
                        :count(0)
                    {
                    }

                    void inc() { count++; }
                    int getCount() { return count; }
                    void enqueue(int v) { q.push(v); }
                    int dequeue() {
                        int f = q.front(); q.pop();
                        return f;
                    }
            };

            typedef std::unique_ptr<Item> Item_uptr;

        public:
            void static encode(std::istream &streamin, std::ostream &streamout)
            {
                std::istreambuf_iterator<char> eos;
                std::string buffer(std::istreambuf_iterator<char>(streamin), eos);
                std::stringstream bufferb;

                CircularSuffixArray cas(buffer);
                int first = 0;

                for (int i = 0; i < buffer.size(); i++)
                {
                    if (cas.index(i) == 0)
                        first = i;
                    bufferb.put(buffer.at((buffer.length() + cas.index(i) - 1) % buffer.length()));
                }
                streamout.write(reinterpret_cast<const char *>(&first), sizeof(first));
                streamout << bufferb.rdbuf();
                streamout.flush();
            }

            // apply Burrows-Wheeler decoding, reading from standard input and writing to standard output
            void static decode(std::istream &streamin, std::ostream &streamout)
            {
                std::vector<Item_uptr> pos(256);

                int first = 0;
                streamin.read(reinterpret_cast<char *>(&first), sizeof(first));

                int i, m, len;
                i = 0;
                m = 0;

                char c;
                while (streamin.get(c)) {
                    if (pos[c] == nullptr)
                        pos[c] = Item_uptr(new Item());
                    pos[c]->inc();
                    pos[c]->enqueue(i);
                    i++;
                }

                std::vector<int> next(i);
                std::vector<char> sorted(i);

                len = i;
                i = 0;

                for (int j = 0; j < 256; j++)
                    for (int k = 0; pos[j] != nullptr && k < pos[j]->getCount(); k++)
                    {
                        next[i] = pos[j]->dequeue();
                        sorted[i] = (char) j;
                        i++;
                    }

                i = first;
                do {
                    streamout.put(sorted[i]);
                    i = next[i];
                    m++;
                } while (m < len);

                streamout.flush();
            }
    };
}
#endif

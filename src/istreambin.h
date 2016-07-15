#ifndef _ISTREAMBIN_H_
#define _ISTREAMBIN_H_

#include <string>
#include <iostream>

namespace bw
{
    class istreambin {
        private:
            char bufferIn;
            int  bufferInBitSize;
            bool bufferInEOF;
            std::istream* in_ptr;

        public:

            istreambin(std::istream* _in_ptr):
                bufferIn(0),
                bufferInBitSize(0),
                bufferInEOF(false),
                in_ptr(_in_ptr)
            {
            }


            void fillbuffer();

            bool read(bool &bit);
            bool read(char &byte);
            bool read(char* s, const int len);
            bool read(std::string &s);
            bool isEmpty();
    };
}

#endif

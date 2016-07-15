#ifndef _OSTREAMBIN_H_
#define _OSTREAMBIN_H_

#include <string>
#include <iostream>

namespace bw
{
    class ostreambin {
        private:
            char bufferOut;
            int  bufferOutBitSize;
            std::ostream* out_ptr;

            void clearbuffer();

        public:
            ostreambin(std::ostream* _out_ptr):
                bufferOut(0),
                bufferOutBitSize(0),
                out_ptr(_out_ptr)
            {
            }

            void write(const bool bit);
            void write(const char byte);
            void write(const char* s, const int len);
            void write(const std::string s);
            void flush();
    };
}

#endif

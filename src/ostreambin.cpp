#include "ostreambin.h"


void bw::ostreambin::flush()
{
    clearbuffer();
    out_ptr->flush();
}

void bw::ostreambin::write(const bool bit)
{
    bufferOut = (bufferOut << 1) | bit;
    bufferOutBitSize++;

    if (bufferOutBitSize == 8) clearbuffer();
}

void bw::ostreambin::write(const char byte)
{
    // optimized if byte-aligned
    if (bufferOutBitSize == 0) {
        out_ptr->put(byte);
        return;
    }

    // otherwise write one bit at a time
    unsigned char c = byte;
    for (int i = 0; i < 8; i++) {
        bool bit = ((c >> (8 - i - 1)) & 1) == 1;
        write(bit);
    }
}

void bw::ostreambin::write(const char* s, const int len)
{
    for (int i=0; i < len; ++i)
        write(s[i]);
}

void bw::ostreambin::write(const std::string s)
{
    for (int i = 0; i < s.size(); ++i)
        write(s.at(i));
}

void bw::ostreambin::clearbuffer()
{
    if (bufferOutBitSize == 0) return;
    if (bufferOutBitSize > 0) bufferOut = bufferOut << (8 - bufferOutBitSize);

    char c = bufferOut;

    out_ptr->put(c);
    bufferOutBitSize = 0;
    bufferOut = 0;
}


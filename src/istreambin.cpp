#include "istreambin.h"

void bw::istreambin::fillbuffer()
{
    if (in_ptr->get(bufferIn))
        bufferInBitSize = 8;
    else
    {
        bufferInBitSize = -1;
        bufferIn = 0;
        bufferInEOF = true;
    }
}

bool bw::istreambin::isEmpty()
{
    return bufferInEOF;
}

bool bw::istreambin::read(bool &bit)
{
    if (bufferInEOF)
        return false;

    --bufferInBitSize;

    bit = ((bufferIn >> bufferInBitSize) & 1);

    if (bufferInBitSize == 0)
        fillbuffer();

    return true;
}

bool bw::istreambin::read(char &byte)
{
    if (bufferInEOF)
        return false;

    if (bufferInBitSize == 8)
    {
        byte = bufferIn;
        fillbuffer();
        return true;
    }

    byte = bufferIn << (8 - bufferInBitSize);
    int oldval = bufferInBitSize;
    fillbuffer();
    if(bufferInEOF)
        return false;
    bufferInBitSize = oldval;
    byte = byte | (((0xff) & bufferIn) >> bufferInBitSize);
    return true;
}

bool bw::istreambin::read(char *s, const int len)
{
    for (int i = 0; i < len && read(s[i]); ++i);
    return !bufferInEOF;
}

bool bw::istreambin::read(std::string &s)
{
    if (bufferInBitSize == 0)
    {
        std::istreambuf_iterator<char> eos;
        std::string s1(std::istreambuf_iterator<char>(*in_ptr), eos);
        s = s1;
        return true;
    }
    return false;
}

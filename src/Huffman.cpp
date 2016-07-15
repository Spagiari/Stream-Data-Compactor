#include "Huffman.h"

const int  bw::Huffman::R;

bool bw::operator<(const bw::Node_ptr &a, const bw::Node_ptr &b)
{
    return a->compareTo(b.get()) < 0;
}

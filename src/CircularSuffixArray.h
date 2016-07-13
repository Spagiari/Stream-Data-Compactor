#ifndef _CIRCULARSUFFIXARRAY_H_
#define _CIRCULARSUFFIXARRAY_H_

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Quick3stringEx.h"

namespace bw {
    class CircularSuffixArray {
        private:
            int len;
            std::shared_ptr<std::vector<int>> idx;
            std::shared_ptr<std::string> b;

        public:
            CircularSuffixArray(const std::string &s)  // circular suffix array of s
                :len(s.size())
                ,idx(new std::vector<int>(len))
                ,b(new std::string(s+s))
            {
                for (int i = 0; i < len; i++)
                    idx->at(i) = i;

                Quick3stringEx quick3Str;

                quick3Str.sort((*idx), (*b));
            }
            int length()                   // length of s
            {
                return len;
            }
            int index(int i)               // returns index of ith sorted suffix
            {   if (i < 0 || i >= len)
                throw new std::invalid_argument("Out of range");
                return idx->at(i);
            }
            std::string strIndex(int i)        // return string on index
            {
                return b->substr(idx->at(i), len + idx->at(i));
            }
    };
}
#endif

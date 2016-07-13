#include "CircularSuffixArray.h"
#include <iostream>

int main(int argc, char** argv)
{
    bw::CircularSuffixArray c("ABRACADABRA!");
    for (int i = 0; i < 12; i++)
    {
        std::cout << c.strIndex(i) << ", " << c.index(i) << std::endl;
    }



    return 0;
}

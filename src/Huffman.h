#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <functional>
#include <string>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <queue>
#include <memory>
#include <iostream>

#include "ostreambin.h"
#include "istreambin.h"

namespace bw {
    class Node
    {
        public:
            unsigned char ch;
            int freq;
            std::unique_ptr<Node> left, right;

            Node(char _ch, int _freq, std::unique_ptr<Node> &_left, std::unique_ptr<Node> &_right)
                :ch(_ch)
                 ,freq(_freq)
                 ,left(std::move(_left))
                 ,right(std::move(_right))
            {
            }

            Node(char _ch, int _freq, std::unique_ptr<Node> &&_left, std::unique_ptr<Node> &&_right)
                :ch(_ch)
                 ,freq(_freq)
                 ,left(std::move(_left))
                 ,right(std::move(_right))
            {
            }

            Node(char _ch, int _freq)
                :ch(_ch)
                 ,freq(_freq)
            {
            }

            Node(std::unique_ptr<Node> & that)
                :ch(that->ch)
                 ,freq(that->freq)
                 ,left(std::move(that->left))
                 ,right(std::move(that->right))
        {
        }

            // compare, based on frequency
            int compareTo(const Node &that) const
            {
                return this->freq - that.freq;
            }

            int compareTo(const Node *that) const
            {
                return this->freq - that->freq;
            }

            // is the node a leaf node?
            bool isLeaf() {
                assert (((left == nullptr) && (right == nullptr)) || ((left != nullptr) && (right != nullptr)));
                return (left == nullptr) && (right == nullptr);
            }

    };
    typedef unsigned char UCHAR;
    typedef std::unique_ptr<Node> Node_ptr;


    bool operator<(const bw::Node_ptr &a, const bw::Node_ptr &b);

    class Huffman {

        // alphabet size of extended ASCII
        private:
            static const int R = 256;

        private:
            // Do not instantiate.
            Huffman()
            {
            }

            // Huffman trie node
            /*
             * Reads a sequence of 8-bit bytes from standard input; compresses them
             * using Huffman codes with an 8-bit alphabet; and writes the results
             * to standard output.
             */
        public:
            void static compress(istreambin &streamin, ostreambin &streamout) {
                // read the input
                std::string input(std::istreambuf_iterator<char>(*(streamin.getStream())), {});

                // tabulate frequency counts
                std::vector<int> freq(R);
                for (int i = 0; i < input.size(); i++)
                {
                    assert(((UCHAR)input[i]) >= 0);
                    freq[((UCHAR)input[i])]++;
                }

                // build Huffman trie
                Node_ptr root(buildTrie(freq));

                // build code table
                std::vector<std::string> st(R);
                buildCode(st, root, "");

                // print trie for decoder
                writeTrie(root, streamout);

                // print number of bytes in original uncompressed message
                int input_size = input.size();
                streamout.write(reinterpret_cast<const char*>(&input_size), sizeof(input_size));

                // use Huffman code to encode input
                for (int i = 0; i < input.size(); i++) {
                    std::string code = st[((UCHAR)input[i])];
                    for (int j = 0; j < code.size(); j++) {
                        if (code.at(j) == '0') {
                            streamout.write(false);
                        }
                        else if (code.at(j) == '1') {
                            streamout.write(true);
                        }
                        else
                        {
                            std::cout << "Code:" << code << std::endl;
                            throw std::invalid_argument("Illegal state");
                        }
                    }
                }

                // close output stream
                streamout.flush();
            }

        private:
            // build the Huffman trie given frequencies
            static Node *buildTrie(const std::vector<int> &freq) {
                // initialze priority queue with singleton trees
                std::priority_queue<Node_ptr, std::vector<Node_ptr>, std::greater<Node_ptr>> pq;

                for (int i = 0; i < R; i++)
                    if (freq[i] > 0)
                        pq.emplace(new Node(i, freq[i]));
                // special case in case there is only one character with a nonzero frequency
                if (pq.size() == 1) {
                    if (freq['\0'] == 0) pq.emplace(new Node('\0', 0));
                    else                 pq.emplace(new Node('\1', 0));
                }

                // merge two smallest trees
                while (pq.size() > 1) {
                    Node_ptr left(std::move(const_cast<Node_ptr&>(pq.top()))); pq.pop();
                    Node_ptr right(std::move(const_cast<Node_ptr&>(pq.top()))); pq.pop();
                    auto freq = left->freq + right->freq;
                    pq.emplace(new Node('\0', freq, left, right));
                }

                Node_ptr ret = std::move(const_cast<Node_ptr&>(pq.top())); pq.pop();
                return ret.release();
            }

            // write bitstring-encoded trie to standard output
            void static writeTrie(const Node_ptr &x, ostreambin &streamout) {
                if (x->isLeaf()) {
                    //BinaryStdOut.write(true);
                    //BinaryStdOut.write(x.ch, 8);
                    streamout.write(true);
                    streamout.write((char)x->ch);
                    return;
                }

                streamout.write(false);
                writeTrie(x->left, streamout);
                writeTrie(x->right, streamout);
            }

            // make a lookup table from symbols and their encodings
            void static buildCode(std::vector<std::string> &st, Node_ptr const &x, const std::string &s) {
                if (!x->isLeaf()) {
                    buildCode(st, x->left,  s + "0");
                    buildCode(st, x->right, s + "1");
                }
                else {
                    st[x->ch] = s;
                }
            }

            /**
             * Reads a sequence of bits that represents a Huffman-compressed message from
             * standard input; expands them; and writes the results to standard output.
             **/
        public:
                            void static expand(istreambin &streamin, ostreambin &streamout) {

                                streamin.fillbuffer();

                                // read in Huffman trie from input stream
                                Node_ptr root(readTrie(streamin));

                                // number of bytes to write
                                int length;

                                streamin.read(reinterpret_cast<char *>(&length), sizeof(length));

                                std::stringstream ss;

                                // decode using the Huffman trie
                                for (int i = 0; i < length; i++) {
                                    Node *x = root.get();
                                    while (!x->isLeaf()) {
                                        bool bit;
                                        streamin.read(bit);

                                        if (bit) x = x->right.get();
                                        else     x = x->left.get();
                                    }
                                    //streamout.getStream()->put(x->ch);
                                    ss.put(x->ch);
                                }
                                (*streamout.getStream()) << ss.rdbuf();
                                streamout.flush();
                            }

        private:
                            static Node *readTrie(istreambin &streamin) {
                                bool isLeaf;

                                streamin.read(isLeaf);
                                Node_ptr ret;

                                if (isLeaf) {
                                    char c;
                                    streamin.read(c);
                                    ret.reset(new Node(c, -1));
                                }
                                else {
                                    Node_ptr left(readTrie(streamin));
                                    Node_ptr right(readTrie(streamin));
                                    ret.reset(new Node('\0', -1, left, right));
                                }

                                return ret.release();
                            }
    };
}
#endif

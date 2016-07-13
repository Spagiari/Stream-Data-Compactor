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

namespace bw {

    class Huffman {

        // alphabet size of extended ASCII
        private:
            static const int R = 256;

            // Do not instantiate.
            Huffman() { }

            // Huffman trie node
        public:
            class Node
            {
                public:
                    typedef std::shared_ptr<Node> Node_ptr;
                    char ch;
                    int freq;
                    Node_ptr left, right;

                    Node(char _ch, int _freq, const Node_ptr _left, const Node_ptr _right)
                        :ch(_ch)
                         ,freq(_freq)
                         ,left(_left)
                         ,right(_right)
                {
                }

                    Node(Node_ptr const & that)
                        :ch(that->ch)
                         ,freq(that->freq)
                         ,left(that->left)
                         ,right(that->right)
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
                        assert (((left == nullptr) && (right == nullptr)) || ((left.get() == nullptr) && (right.get() == nullptr)));
                        return (left.get() == nullptr) && (right.get() == nullptr);
                    }

            };
            typedef std::shared_ptr<Node> Node_ptr;

            /**
             * Reads a sequence of 8-bit bytes from standard input; compresses them
             * using Huffman codes with an 8-bit alphabet; and writes the results
             * to standard output.
             */

            void static compress(std::istream &streamin, std::ostream &streamout) {
                // read the input
                std::istreambuf_iterator<char> eos;
                std::string s(std::istreambuf_iterator<char>(streamin), eos);

                std::vector<char> input(s.begin(), s.end());

                // tabulate frequency counts
                std::vector<int> freq(R);
                for (int i = 0; i < input.size(); i++)
                    freq[input[i]]++;

                // build Huffman trie
                Node root = buildTrie(freq);

                // build code table
                std::vector<std::string> st(R);
                buildCode(st, root, "");

                // print trie for decoder
                writeTrie(root, streamout);
                // print number of bytes in original uncompressed message
                streamout << input.size();

                // use Huffman code to encode input
                for (int i = 0; i < input.size(); i++) {
                    std::string code = st[input[i]];
                    for (int j = 0; j < code.size(); j++) {
                        if (code.at(j) == '0') {
                            streamout << false;
                        }
                        else if (code.at(j) == '1') {
                            streamout << true;
                        }
                        else throw new std::invalid_argument("Illegal state");
                    }
                }

                // close output stream
                streamout.flush();
            }

        private:
            // build the Huffman trie given frequencies
            Node static buildTrie(const std::vector<int> &freq) {
                // initialze priority queue with singleton trees
                std::priority_queue<Node_ptr, std::vector<Node_ptr>, std::greater<Node_ptr>> pq;

                for (int i = 0; i < R; i++)
                    if (freq[i] > 0)
                        pq.push(Node_ptr(new Node(i, freq[i], nullptr, nullptr)));
                // special case in case there is only one character with a nonzero frequency
                if (pq.size() == 1) {
                    if (freq['\0'] == 0) pq.push(Node_ptr(new Node('\0', 0, nullptr, nullptr)));
                    else                 pq.push(Node_ptr(new Node('\1', 0, nullptr, nullptr)));
                }

                // merge two smallest trees
                while (pq.size() > 1) {
                    Node_ptr left  = pq.top(); pq.pop();
                    Node_ptr right = pq.top(); pq.pop();
                    Node_ptr parent = Node_ptr(new Node('\0', left->freq + right->freq, left, right));
                    pq.push(parent);
                }

                Node ret(pq.top()); pq.pop();
                return ret;
            }

            // write bitstring-encoded trie to standard output
            void static writeTrie(Node x, std::ostream &streamout) {
                if (x.isLeaf()) {
                    //BinaryStdOut.write(true);
                    //BinaryStdOut.write(x.ch, 8);
                    streamout << true;
                    streamout << x.ch;
                    return;
                }
                streamout << false;
                writeTrie(x.left, streamout);
                writeTrie(x.right, streamout);
            }

            // make a lookup table from symbols and their encodings
            void static buildCode(std::vector<std::string> &st, Node x, const std::string &s) {
                if (!x.isLeaf()) {
                    buildCode(st, x.left,  s + "0");
                    buildCode(st, x.right, s + "1");

                }
                else {
                    st[x.ch] = s;
                }
            }

            /**
             * Reads a sequence of bits that represents a Huffman-compressed message from
             * standard input; expands them; and writes the results to standard output.
             */
        public:
                        void static expand(std::istream &streamin, std::ostream &streamout) {

                            // read in Huffman trie from input stream
                            Node root = readTrie(streamin);

                            // number of bytes to write
                            int length;

                            streamin >> length;

                            // decode using the Huffman trie
                            for (int i = 0; i < length; i++) {
                                Node x = root;
                                while (!x.isLeaf()) {
                                    bool bit;
                                    streamin >> bit;

                                    if (bit) x = x.right;
                                    else     x = x.left;
                                }
                                streamout << x.ch;
                            }
                            streamout.flush();
                        }

        private:
                        Node_ptr static readTrie(std::istream &streamin) {
                            bool isLeaf;
                            streamin >> isLeaf;

                            if (isLeaf) {
                                return Node_ptr(new Node(streamin.get(), -1, nullptr, nullptr));
                            }
                            else {
                                return Node_ptr(new Node('\0', -1, readTrie(streamin), readTrie(streamin)));
                            }
                        }
    };

    bool operator<(const Huffman::Node_ptr &a, const Huffman::Node_ptr &b)
    {
        return a->compareTo(b.get()) < 0;
    }
}

#endif

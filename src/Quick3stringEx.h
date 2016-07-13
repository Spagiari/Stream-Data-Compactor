#ifndef _QUICK3STRINGEX_H_
#define _QUICK3STRINGEX_H_

#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <memory>

#include <boost/noncopyable.hpp>

namespace bw {
    class Quick3stringEx : boost::noncopyable {
        private:
            static const int CUTOFF =  15;   // cutoff to insertion sort
            std::shared_ptr<std::string> buff;
            int bufflen;
        public:
            Quick3stringEx() { };
            /**
             * Rearranges the array of strings in ascending order.
             *
             * @param a the array to be sorted
             */

            void sort(std::vector<int> &a, const std::string &b) {
                buff = std::shared_ptr<std::string>(new std::string(b));
                bufflen = b.size();
                std::random_shuffle ( a.begin(), a.end() );
                sort(a, 0, a.size()-1, 0);
                assert(isSorted(a));
            }

            std::string getString(int base, int d) {
                return buff->substr(base, base + d);
            }

            //return the dth character of s, -1 if d = length of s
        private:
            int  charAt(int base, int d) {
                assert(d >= 0 && d <= bufflen);
                if (d == bufflen) return -1;
                return buff->at(base + d);
            }

            //3-way string quicksort a[lo..hi] starting at dth character
            void sort(std::vector<int>& a, int lo, int hi, int d) {
                //
                // cutoff to insertion sort for small subarrays
                if (hi <= lo + CUTOFF) {
                    insertion(a, lo, hi, d);
                    return;
                }
                int lt = lo, gt = hi;
                int v = charAt(a[lo], d);
                int i = lo + 1;
                while (i <= gt) {
                    int t = charAt(a[i], d);
                    if (t < v) exch(a, lt++, i++);
                    else if (t > v) exch(a, i, gt--);
                    else i++;
                }

                // a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi].

                sort(a, lo, lt-1, d);
                if (v >= 0) sort(a, lt, gt, d+1);
                sort(a, gt+1, hi, d);
            }

            // sort from a[lo] to a[hi], starting at the dth character
            void insertion(std::vector<int> &a, int lo, int hi, int d) {
                for (int i = lo; i <= hi; i++)
                    for (int j = i; j > lo && less(a[j], a[j-1], d); j--)
                        exch(a, j, j-1);
            }


            // exchange a[i] and a[j]
            void exch(std::vector<int> &a, int i, int j) {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }

            // is v less than w, starting at character d
            // DEPRECATED BECAUSE OF SLOW SUBSTRING EXTRACTION IN JAVA 7
            // private static boolean less(String v, String w, int d) {
            //    assert v.substring(0, d).equals(w.substring(0, d));
            //    return v.substring(d).compareTo(w.substring(d)) < 0;
            // }

            // is v less than w, starting at character d
            bool less(int v, int w, int d) {
                //assert getString(v, d).equals(getString(w, d));
                for (int i = d; i < bufflen; i++) {
                    if (charAt(v, i) < charAt(w, i)) return true;
                    if (charAt(v, i) > charAt(w, i)) return false;
                }
                return false;
            }

            // is the array sorted
            bool isSorted(std::vector<int> &a) {
                for (int i = 1; i < a.size(); i++)
                    if (getString(a[i], bufflen).compare(getString(a[i-1], bufflen)) < 0) return false;
                return true;
            }
    };
}
#endif

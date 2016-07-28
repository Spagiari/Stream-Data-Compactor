#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "Huffman.h"
#include "istreambin.h"
#include "ostreambin.h"
#include <sstream>

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;

} // namespace

int main(int argc, char** argv)
{
    //std::string sif;
    //std::string sof;
    try
    {
        /** Define and parse the program options
         *      */
        std::string appName = boost::filesystem::basename(argv[0]);

        namespace po = boost::program_options;
        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "Print help messages")
            ("encode,e", "Apply move to front encode")
            ("decode,d", "Apply move to front decode")
            //("input,i", po::value<std::string>(&sif)->required(),  "Input File")
            //("output,o", po::value<std::string>(&sof)->required(), "Output File")
            ("hexdump,x", "Set output to hex format");

        po::variables_map vm;
        try
        {
            po::store(po::parse_command_line(argc, argv, desc),
                    vm); // can throw

            /** --help option
             *        */
            if ( vm.count("help")  )
            {
                std::cout << "Move to front command line tool" << std::endl <<
                    "Usage: " << appName << " [-h+-] [-x]" << std::endl << desc;
                return SUCCESS;
            }

            po::notify(vm); // throws on error, so do after help in case
            // there are any problems
        }
        catch(po::error& e)
        {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << desc << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }

        //std::fstream ifs;
        //ifs.open(sif.c_str(), std::ios::binary | std::ios::in);
        //std::fstream ofs;
        //ofs.open(sof.c_str(), std::ios::binary | std::ios::out);

        if (vm.count("encode"))
        {
            if (vm.count("hexdump"))
            {
                std::stringstream sout;
                bw::ostreambin streamout(&sout);
                bw::istreambin streamin(&std::cin);
                bw::Huffman::compress(streamin, streamout);
                int bytes = 0;
                char c;
                while (sout.get(c))
                {
                    std::cout << std::hex << std::setfill('0') << std::setw(2) << int(0xff & ((int) c)) << " ";
                    bytes++;
                }

                std::cout.clear();
                std::cout << std::endl << std::dec << bytes * 8 << " bits" << std::endl;
            }
            else
            {
                bw::ostreambin streamout(&std::cout);
                bw::istreambin streamin(&std::cin);

                bw::Huffman::compress(streamin, streamout);
            }
        }

        if (vm.count("decode"))
        {

            bw::ostreambin streamout(&std::cout);
            std::string s(std::istreambuf_iterator<char>(std::cin), {});
            std::stringstream in(s);
            bw::istreambin streamin(&in);
            bw::Huffman::expand(streamin, streamout);
        }

    }
    catch(std::exception& e)
    {
        std::cerr << "Unhandled Exception reached the top of main: "
            << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;

    }

    return SUCCESS;

} // main

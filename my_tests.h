#pragma once
// BEGINNOLINT
#include "CLA/cla_parser.h"
#include "Exceptions/exceptions.h"
#include <cassert>
#include <sstream>

int CLAChecking(int argc, char **argv) {
    try {
        // обработка аргументов
        CLAParser args(
            "A program for compressing multiple files and decompressing a single file using the Huffman algorithm.\n"
            "Performed by Mikhail Tropin (Aris)");
        std::vector<std::string> input_files;
        std::string output_file;
        std::string archive_file;

        args.AddMultipleArgument('c', "compress", "archive_name file1 [file2 ...]",
                                 "compress files file1 [file2..] to file archive_name", &output_file, &input_files);
        args.AddUniqueArgument('d', "decompress", "archive_name", "decompress file archive_name", &archive_file);
        args.AddFlag('h', "help", "Prints help message with usage");

        args.Parse(argc, argv);

        if (args.ToPerform("help")) {
            if (args.ToPerform("compress")) {
                std::cerr << "Expected unique argument, but -h and -c found" << std::endl;
                return -4;
            }
            if (args.ToPerform("decompress")) {
                std::cerr << "Expected unique argument, but -h and -d found" << std::endl;
                return -5;
            }
            return 1;
        } else if (args.ToPerform("compress")) {
            if (args.ToPerform("decompress")) {
                std::cerr << "Expected unique argument, but -c and -d found" << std::endl;
                throw InvalidArguments();
            }
            if (input_files.empty()) {
                std::cerr << "Expected at least one input_file for compression but zero found" << std::endl;
                throw InvalidArguments();
            }
            return 2;
        } else if (args.ToPerform("decompress")) {

            return 3;
        } else {
            std::cerr << "Expected exactly one argument but zero found" << std::endl;
            return -6;
        }
    } catch (InvalidArguments e) {
        std::cerr << "Invalid Argument Exception occurred" << std::endl;
        return -1;
    } catch (IllegalStateException e) {
        std::cerr << "Illegal State Exception occurred" << std::endl;
        return -2;
    } catch (std::exception e) {
        std::cerr << "Unknown exception\n" << e.what() << std::endl;
        return -3;
    }
    return 0;
}

void TestCLA() {
    {
        char *argv[] = {(char *)"archiver", (char *)"-h"};
        assert(CLAChecking(2, argv) == 1);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-c", (char *)"file1"};
        assert(CLAChecking(3, argv) == -1);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-c", (char *)"file1", (char *)"file2"};
        assert(CLAChecking(4, argv) == 2);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-d", (char *)"file1"};
        assert(CLAChecking(3, argv) == 3);
    }
    {
        char *argv[] = {(char *)"archiver"};
        assert(CLAChecking(1, argv) == -6);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-c"};
        assert(CLAChecking(2, argv) == -1);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-ch"};
        assert(CLAChecking(2, argv) == -1);
    }
    {
        char *argv[] = {(char *)"archiver", (char *)"-x"};
        assert(CLAChecking(2, argv) == -1);
    }
    //    std::cout << CLAChecking(1, (const char**)("dsf")) << std::endl;
    //    std::cout << CLAChecking(2, (char**)({"ac""-h";})) << std::endl;
    //    std::cout << CLAChecking(2, (char**)({"archiver", "-c file1 file2";})) << std::endl;
    //    std::cout << CLAChecking(1, (char**)({"archiver", "-hc";})) << std::endl;
}

void TestIO() {
    std::string ans;
    {
        std::ostringstream os;
        Writer writer(os);
        Symbol symbol("00100110");  //  00100110 -> 38
        writer.WriteSymbol(symbol);
        writer.Flush();
        ans += static_cast<char>(38);
        assert(os.str() == ans);
        ans.clear();
    }
    {
        std::ostringstream os;
        Writer writer(os);
        Symbol symbol("1100");  // 1100 -> 11000000 = 192
        writer.WriteSymbol(symbol);
        writer.Flush();
        ans += static_cast<char>(192);
        assert(os.str() == ans);
        ans.clear();
    }
    {
        std::ostringstream os;
        Writer writer(os);
        Symbol symbol(
            "00101011"
            "001001");  //  00101011 = 43, 001001 -> 00100100 = 36
        writer.WriteSymbol(symbol);
        writer.Flush();
        ans += static_cast<char>(43);
        ans += static_cast<char>(36);
        assert(os.str() == ans);
    }
    {
        std::stringstream ss;

        char a = 97;

        ss.write(&a, sizeof(a));
        Reader reader(ss);
        Symbol symbol;
        reader.ReadSymbol(symbol, 8u);
        assert(symbol == Symbol("01100001"));
    }
}

void Test() {
    TestCLA();
    TestIO();

    /*
     * Usage: #inlclude "my_tests.h" in archiver.cpp
     * call Test(); in main()
     */
}
// ENDNOLINT
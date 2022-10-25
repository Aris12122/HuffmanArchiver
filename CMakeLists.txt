add_executable(
        archiver
        archiver.cpp
        CLA/cla_parser.h CLA/cla_parser.cpp
        init.h
        Symbol/symbol.h Symbol/symbol.cpp
        IO/reader.h IO/reader.cpp
        IO/writer.h IO/writer.cpp
        Huffman/encoder.h Huffman/encoder.cpp
        Huffman/huffman.h Huffman/huffman.cpp
        Huffman/decoder.h Huffman/decoder.cpp
)

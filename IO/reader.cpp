//
// Created by aris on 10/21/22.
//
#include "reader.h"

Reader::Reader(std::istream& input) : input_(input) {
    buff_ = 0;
    cur_pos_ = 0u;
}

bool Reader::ReadSymbol(Symbol& symbol, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        symbol.bit_seq_.emplace_back(ReadBit());
    }
    return input_.good();
}
bool Reader::ReadBit() {
    if (cur_pos_ == 0u) {
        buff_ = 0;
        if (input_.good()) {
            input_.read(&buff_, 1u);
        }
        cur_pos_ = BUFFER_SIZE;
    }
    return (buff_ >> (--cur_pos_)) & 1;
}

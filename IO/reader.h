//
// Created by aris on 10/20/22.
//
#pragma once
#include "../init.h"
#include "istream"

class Reader {
public:
    explicit Reader(std::istream& input);

    bool ReadSymbol(Symbol& symbol, size_t len);

    bool ReadBit();

private:
    std::istream& input_;
    char buff_;
    size_t cur_pos_;
};
//
// Created by aris on 10/20/22.
//
#pragma once
#include <bitset>
#include <ostream>
#include "../init.h"

class Writer {
public:
    explicit Writer(std::ostream& output);

    void WriteSymbol(const Symbol& symbol);

    void WriteBit(bool x);

    void Flush();

    ~Writer();

private:
    void Print();

    std::ostream& output_;
    size_t len_;
    int buff_;
};
//
// Created by aris on 10/21/22.
//
#include "writer.h"

Writer::Writer(std::ostream& output) : output_(output), len_(BUFFER_SIZE), buff_(0) {
}
void Writer::WriteSymbol(const Symbol& symbol) {
    for (size_t i = 0; i < symbol.Size(); ++i) {
        WriteBit(symbol[i]);
    }
}
void Writer::WriteBit(bool x) {
    buff_ |= (x << (--len_));
    if (len_ == 0u) {
        Print();
    }
}
Writer::~Writer() {
    Flush();
}
void Writer::Print() {
    output_ << static_cast<char>(buff_);
    buff_ = 0;
    len_ = BUFFER_SIZE;
}
void Writer::Flush() {
    if (len_ < BUFFER_SIZE) {
        Print();
    }
}

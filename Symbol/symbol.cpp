//
// Created by aris on 10/20/22.
//
#include "symbol.h"
#include "../Exceptions/exceptions.h"
#include "../init.h"

Symbol::Symbol(size_t x) {
    while (x) {
        bit_seq_.emplace_back(x % 2);
        x >>= 1;
    }
    if (bit_seq_.empty()) {
        bit_seq_.emplace_back(false);
    }
    std::reverse(bit_seq_.begin(), bit_seq_.end());
}
Symbol::Symbol(const std::vector<bool>& vb) : bit_seq_(vb) {
}
Symbol::Symbol(const Symbol& other, size_t len) {
    bit_seq_ = other.bit_seq_;
    if (bit_seq_.size() != len) {
        std::reverse(bit_seq_.begin(), bit_seq_.end());
        while (bit_seq_.size() < len) {
            bit_seq_.emplace_back(false);
        }
        while (bit_seq_.size() > len) {
            bit_seq_.pop_back();
        }
        std::reverse(bit_seq_.begin(), bit_seq_.end());
    }
}
Symbol::Symbol(const std::string& b_str) {
    bit_seq_.resize(b_str.size());
    for (size_t i = 0; i < bit_seq_.size(); ++i) {
        if (b_str[i] < '0' || b_str[i] > '1') {
            std::cerr << "Expected binary string for Symbol constructor, but " << b_str << " found" << std::endl;
            throw IllegalStateException();
        }
        bit_seq_[i] = static_cast<bool>(b_str[i] - '0');
    }
}
Symbol::Symbol(size_t x, size_t len) : Symbol(x) {
    bit_seq_.resize(len);
    for (size_t i = len; i > 0; --i) {
        bit_seq_[i - 1] = x & 1;
        x >>= 1;
    }
    if (x > 0) {
        std::cerr << "Impossible to create Symbol with length " << len << std::endl;
        throw IllegalStateException();
    }
}
Symbol::Symbol() {
    bit_seq_ = std::vector<bool>(0);
}
bool Symbol::operator[](size_t pos) const {
    return bit_seq_[pos];
}
bool Symbol::operator<(const Symbol& other) const {
    if (Size() == other.Size()) {
        return bit_seq_ < other.bit_seq_;
    }
    if (Size() < other.Size()) {
        Symbol a(*this, other.Size());
        return a.bit_seq_ < other.bit_seq_;
    } else {
        Symbol a(other, Size());
        return bit_seq_ < a.bit_seq_;
    }
}
bool Symbol::operator!=(const Symbol& other) const {
    return bit_seq_ != other.bit_seq_;
}
bool Symbol::operator==(const Symbol& other) const {
    return bit_seq_ == other.bit_seq_;
}
bool Symbol::IsEmpty() const {
    return bit_seq_.empty();
}
size_t Symbol::Size() const {
    return bit_seq_.size();
}
void Symbol::Reverse() {
    std::reverse(bit_seq_.begin(), bit_seq_.end());
}
void Symbol::Add(bool x) {
    bit_seq_.emplace_back(x);
}
Symbol& Symbol::operator++() {
    bool added = false;
    for (size_t i = bit_seq_.size(); i > 0; --i) {
        if (bit_seq_[i - 1]) {
            bit_seq_[i - 1] = false;
        } else {
            bit_seq_[i - 1] = true;
            added = true;
            break;
        }
    }
    if (!added) {
        bit_seq_.insert(bit_seq_.begin(), true);
    }
    return *this;
}
void Symbol::Clear() {
    bit_seq_.clear();
}
size_t Symbol::ToInt() {
    size_t result = 0;
    for (size_t i = 0; i < Size(); ++i) {
        result <<= 1;
        result += bit_seq_[i];
    }
    return result;
}
char Symbol::ToChar() {
    return static_cast<char>(ToInt());
}
std::vector<Symbol> TransformString(const std::string& str) {
    std::vector<Symbol> result;
    for (size_t i = 0; i < str.size(); ++i) {
        result.emplace_back(Symbol(static_cast<size_t>(str[i]), WORD_LEN));
    }
    return result;
}

std::vector<Symbol> TransformFileName(const std::string& file_name) {
    size_t begin = 0u;
    for (size_t i = 0; i < file_name.size(); ++i) {
        if (file_name[i] == '/') {
            begin = i + 1;
        }
    }
    return TransformString(file_name.substr(begin, file_name.size() - begin));
}

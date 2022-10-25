//
// Created by aris on 10/23/22.
//

#include "../Exceptions/exceptions.h"
#include "decoder.h"

Decoder::Decoder(Reader& reader) : reader_(reader) {
}
void Decoder::ReadTrieData() {
    Symbol read;
    reader_.ReadSymbol(read, WORD_LEN);
    symbols_count_ = read.ToInt();
    read.Clear();
    for (size_t i = 0; i < symbols_count_; ++i) {
        reader_.ReadSymbol(read, WORD_LEN);
        ordered_symbols_.emplace_back(read);
        read.Clear();
    }
    size_t cur_symbols_cnt = 0u;
    for (size_t code_size = 1u; cur_symbols_cnt < symbols_count_; ++code_size) {
        reader_.ReadSymbol(read, WORD_LEN);
        while (code_size >= num_code_size_.size()) {
            num_code_size_.emplace_back(0u);
        }
        num_code_size_[code_size] = read.ToInt();
        cur_symbols_cnt += read.ToInt();
        max_code_size_ = code_size;
        read.Clear();
    }
    if (cur_symbols_cnt != symbols_count_) {
        std::cerr << "Symbols count do not equal to sum of symbols count with fixed length" << std::endl;
        throw IllegalStateException();
    }
}
void Decoder::Decode() {
    std::vector<std::pair<Symbol, Symbol>> symbol_code_vec;
    Symbol current_code;
    size_t len = 1u;
    for (const auto& symbol : ordered_symbols_) {
        while (num_code_size_[len] == 0) {
            len++;
        }
        --num_code_size_[len];
        while (current_code.Size() < len) {
            current_code.Add(false);
        }
        symbol_code_vec.emplace_back(symbol, current_code);
        ++current_code;
    }
    BuildTrie(symbol_code_vec);
}
std::string Decoder::ReadFileName() {
    std::string file_name;
    Symbol next;
    while (true) {
        DecodeNext(next, root);
        if (next == FILENAME_END) {
            break;
        }
        file_name += next.ToChar();
    }
    return file_name;
}
bool Decoder::ArchiveEnd() const {
    return archive_end_;
}
void Decoder::PrintDecoded(Writer& writer) {
    while (true) {
        Symbol next;
        DecodeNext(next, root);
        if (next == ARCHIVE_END) {
            archive_end_ = true;
            break;
        } else if (next == ONE_MORE_FILE) {
            break;
        }
        writer.WriteSymbol(Symbol(next, READ_WORD_LEN));
    }
}
void Decoder::DecodeNext(Symbol& symbol, std::unique_ptr<Trie>& vertex) {
    if (IsLeaf(vertex)) {
        symbol = vertex->symbol;
        return;
    }
    if (reader_.ReadBit()) {
        DecodeNext(symbol, vertex->right);
    } else {
        DecodeNext(symbol, vertex->left);
    }
}
Decoder::~Decoder() {
    root.reset();
}

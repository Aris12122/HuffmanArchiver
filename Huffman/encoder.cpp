//
// Created by aris on 10/20/22.
//
#include "encoder.h"
#include <queue>
#include "../Exceptions/exceptions.h"

Encoder::Encoder() {
}
void Encoder::Add(const Symbol& symbol) {
    ++frequency_[symbol];
}
void Encoder::Add(Reader& reader) {
    Symbol symbol;
    while (reader.ReadSymbol(symbol, READ_WORD_LEN)) {
        symbol = Symbol(symbol, WORD_LEN);
        Add(symbol);
        symbol.Clear();
    }
}
void Encoder::Add(const std::vector<Symbol>& symbols) {
    for (const auto& symbol : symbols) {
        Add(symbol);
    }
}
void Encoder::Encode() {
    if (frequency_.empty()) {
        throw IllegalStateException();
    }

    std::vector<std::pair<size_t, Symbol>> symbol_freq_vec;
    for (const auto& [symbol, cnt] : frequency_) {
        symbol_freq_vec.emplace_back(cnt, symbol);
    }
    std::sort(symbol_freq_vec.begin(), symbol_freq_vec.end(), [](const auto& less, const auto& greater) {
        return std::tie(less.first, less.second) < std::tie(greater.first, greater.second);
    });

    using PairCntTrie = std::pair<size_t, std::unique_ptr<Trie>>;

    std::queue<PairCntTrie> symbol_freq;
    for (const auto& [cnt, symbol] : symbol_freq_vec) {
        symbol_freq.push({cnt, std::make_unique<Trie>(symbol)});
    }
    symbol_freq_vec.clear();
    std::queue<PairCntTrie> word_freq;

    auto get_min = [](std::queue<PairCntTrie>& symbol_freq, std::queue<PairCntTrie>& word_freq) {
        PairCntTrie result;
        if (word_freq.empty() ||
            (!symbol_freq.empty() && std::tie(symbol_freq.front().first, symbol_freq.front().second->symbol) <
                                         std::tie(word_freq.front().first, word_freq.front().second->symbol))) {
            result = std::move(symbol_freq.front());  // move
            symbol_freq.pop();
        } else {
            result = std::move(word_freq.front());
            word_freq.pop();
        }
        return result;
    };

    while (symbol_freq.size() + word_freq.size() > 1u) {
        PairCntTrie min1 = get_min(symbol_freq, word_freq);
        PairCntTrie min2 = get_min(symbol_freq, word_freq);

        Symbol mn_symbol = min1.second->symbol;
        if (min2.second->symbol < mn_symbol) {
            mn_symbol = min2.second->symbol;
        }

        PairCntTrie to_add = {min1.first + min2.first,
                              std::make_unique<Trie>(mn_symbol, min1.second.release(), min2.second.release())};
        word_freq.push(std::move(to_add));
    }
    PairCntTrie last = get_min(symbol_freq, word_freq);
    root = std::move(last.second);
    ToCanonical();
}
void Encoder::ToCanonical() {
    std::vector<std::pair<size_t, Symbol>> len_symbol_vec;
    GetLeafs(root, len_symbol_vec);
    root.reset();
    std::sort(len_symbol_vec.begin(), len_symbol_vec.end());

    Symbol current_code;
    for (const auto& [len, symbol] : len_symbol_vec) {
        while (current_code.Size() < len) {
            current_code.Add(false);
        }
        coded_symbol_[symbol] = current_code;
        while (current_code.Size() >= num_code_size_.size()) {
            num_code_size_.emplace_back(0u);
        }
        num_code_size_[current_code.Size()]++;

        if (max_code_size_ < current_code.Size()) {
            max_code_size_ = current_code.Size();
        }
        ordered_symbols_.emplace_back(symbol);
        ++symbols_count_;
        ++current_code;
    }
}
void Encoder::PrintEncoded(const Symbol& symbol, Writer& writer) {
    if (!coded_symbol_.contains(symbol)) {
        std::cerr << "You cannot print encoded Symbol, that has not been added" << std::endl;
        throw IllegalStateException();
    }
    writer.WriteSymbol(coded_symbol_[symbol]);
}
void Encoder::PrintEncoded(const std::vector<Symbol>& symbols, Writer& writer) {
    for (const auto& symbol : symbols) {
        PrintEncoded(symbol, writer);
    }
}
void Encoder::PrintEncoded(Reader& reader, Writer& writer) {
    Symbol symbol;
    while (reader.ReadSymbol(symbol, READ_WORD_LEN)) {
        symbol = Symbol(symbol, WORD_LEN);
        PrintEncoded(symbol, writer);
        symbol.Clear();
    }
}

void Encoder::GetLeafs(std::unique_ptr<Trie>& vertex, std::vector<std::pair<size_t, Symbol>>& leafs_len_symbol,
                       size_t len) {
    if (vertex == NULL) {
        return;
    }
    if (IsLeaf(vertex)) {
        leafs_len_symbol.emplace_back(len, vertex->symbol);
    }
    GetLeafs(vertex->left, leafs_len_symbol, len + 1);
    GetLeafs(vertex->right, leafs_len_symbol, len + 1);
}

void Encoder::PrintTrieData(Writer& writer) {
    writer.WriteSymbol(Symbol(symbols_count_, WORD_LEN));
    for (size_t i = 0; i < symbols_count_; ++i) {
        writer.WriteSymbol(ordered_symbols_[i]);
    }
    for (size_t code_size = 1; code_size <= max_code_size_; ++code_size) {
        writer.WriteSymbol(Symbol(num_code_size_[code_size], WORD_LEN));
    }
}
Encoder::~Encoder() {
    root.reset();
}

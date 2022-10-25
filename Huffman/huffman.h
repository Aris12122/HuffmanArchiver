//
// Created by aris on 10/20/22.
//
#pragma once
#include "../Symbol/symbol.h"
#include <unordered_map>
#include <memory>

class Huffman {

protected:
    Huffman();
    std::vector<size_t> num_code_size_;
    std::unordered_map<Symbol, Symbol> coded_symbol_;
    std::vector<Symbol> ordered_symbols_;

    size_t symbols_count_ = 0u;
    size_t max_code_size_ = 0u;

    struct Trie {
        Trie();

        explicit Trie(const Symbol& symbol);

        explicit Trie(const Symbol& symbol, Trie* l, Trie* r);

        explicit Trie(Trie* l, Trie* r);

        ~Trie();

        Symbol symbol;
        std::unique_ptr<Trie> left;
        std::unique_ptr<Trie> right;
    };
    std::unique_ptr<Trie> root;

    void BuildTrie(std::vector<std::pair<Symbol, Symbol>> symbol_code);
    bool IsLeaf(std::unique_ptr<Trie>& vertex);

    void TrieAddSymbol(std::unique_ptr<Trie>& vertex, const Symbol& code, const Symbol symbol, size_t pos = 0u);
};
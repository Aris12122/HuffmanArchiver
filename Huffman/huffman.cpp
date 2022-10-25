//
// Created by aris on 10/21/22.
//
#include "huffman.h"
#include "iostream"

void Huffman::BuildTrie(std::vector<std::pair<Symbol, Symbol>> symbol_code) {
    root = std::make_unique<Trie>();
    for (const auto &[symbol, code] : symbol_code) {
        TrieAddSymbol(root, code, symbol);
    }
}
void Huffman::TrieAddSymbol(std::unique_ptr<Trie> &vertex, const Symbol &code, const Symbol symbol, size_t pos) {
    if (!vertex) {
        vertex = std::make_unique<Trie>();
    }
    if (pos < code.Size()) {
        if (code[pos]) {
            TrieAddSymbol(vertex->right, code, symbol, pos + 1);
        } else {
            TrieAddSymbol(vertex->left, code, symbol, pos + 1);
        }
    }
    vertex->symbol = symbol;
}
Huffman::Huffman() {
}
bool Huffman::IsLeaf(std::unique_ptr<Trie> &vertex) {
    return vertex->left == NULL && vertex->right == NULL;
}
Huffman::Trie::Trie(const Symbol &symbol) : symbol(symbol) {
}
Huffman::Trie::Trie(const Symbol &symbol, Huffman::Trie *l, Huffman::Trie *r) : symbol(symbol), left(l), right(r) {
}
Huffman::Trie::Trie(Huffman::Trie *l, Huffman::Trie *r) : left(l), right(r) {
}
Huffman::Trie::~Trie() {
    left.reset();
    right.reset();
}
Huffman::Trie::Trie() {
}

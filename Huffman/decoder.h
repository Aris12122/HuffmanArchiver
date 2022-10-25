//
// Created by aris on 10/20/22.
//
#pragma once
#include "../init.h"
#include "../IO/writer.h"
#include "../IO/reader.h"
#include "huffman.h"

class Decoder : public Huffman {
public:
    explicit Decoder(Reader& reader);

    void ReadTrieData();

    void Decode();

    void DecodeNext(Symbol& symbol, std::unique_ptr<Trie>& vertex);

    std::string ReadFileName();

    bool ArchiveEnd() const;

    void PrintDecoded(Writer& writer);

    ~Decoder();

private:
    Reader& reader_;

    bool archive_end_ = false;
};

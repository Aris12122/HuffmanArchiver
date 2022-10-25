//
// Created by aris on 10/20/22.
//
#pragma once
#include "Symbol/symbol.h"

static const size_t WORD_LEN = 9u;
static const size_t READ_WORD_LEN = 8u;
static const size_t BUFFER_SIZE = 8u;

static const Symbol FILENAME_END = Symbol(256u);
static const Symbol ONE_MORE_FILE = Symbol(257u);
static const Symbol ARCHIVE_END = Symbol(258u);

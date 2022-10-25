//
// Created by aris on 10/21/22.
//
#pragma once
#include "exception"
#include "iostream"

class IOSException : public std::exception {};

class IllegalStateException : public std::exception {};

class InvalidArguments : public std::exception {};

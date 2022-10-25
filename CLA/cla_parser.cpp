//
// Created by aris on 10/21/22.
//
#include "cla_parser.h"
#include "../Exceptions/exceptions.h"
#include "iostream"

CLAParser::CLAParser(const std::string program_info) : program_info_(program_info) {
}
void CLAParser::AddUniqueArgument(char short_name, const std::string& long_name, const std::string& type,
                                  const std::string& help, std::string* store_to) {
    args_.emplace_back(ArgumentHolder{
        .short_name = short_name,
        .long_name = long_name,
        .help = help,
        .arg_type = ArgType::Unique,
        .to_perform = false,
        .type = type,
        .argument_value = store_to,
    });
}
void CLAParser::AddMultipleArgument(char short_name, const std::string& long_name, const std::string& type,
                                    const std::string& help, std::string* store_to,
                                    std::vector<std::string>* store_vec_to) {
    args_.emplace_back(ArgumentHolder{
        .short_name = short_name,
        .long_name = long_name,
        .help = help,
        .arg_type = ArgType::Multiple,
        .to_perform = false,
        .type = type,
        .argument_value = store_to,
        .argument_values = store_vec_to,
    });
}
void CLAParser::AddFlag(char short_name, const std::string& long_name, const std::string& help) {
    args_.emplace_back(ArgumentHolder{
        .short_name = short_name,
        .long_name = long_name,
        .help = help,
        .arg_type = ArgType::Flag,
        .to_perform = false,
        .type = "Empty",
    });
}
std::ostream& CLAParser::PrintHelpMessages(std::ostream& output) const {
    output << program_info_ << '\n';
    for (const auto& argument : args_) {
        output << '\n';
        output << "-" << argument.short_name << ' ' << "--" << argument.long_name << '\n';
        output << argument.type << " | " << argument.help << '\n';
    }
    return output;
}
bool CLAParser::ToPerform(const std::string& long_name) const {
    for (const auto& argument : args_) {
        if (argument.long_name == long_name) {
            return argument.to_perform;
        }
    }
    std::cerr << long_name << " doesn't match any of arguments" << std::endl;
    throw InvalidArguments();
}
void CLAParser::Parse(int argc, char** argv) {
    for (size_t pos = 1; pos < argc; ++pos) {
        if (argv[pos][0] != '-') {
            std::cerr << "Expected argument_name (with '-' or '--'), but " << argv[pos] << " found" << std::endl;
            throw InvalidArguments();
        }
        bool found = false;
        for (auto& argument : args_) {
            if ((argv[pos] == "-" + std::string(1, argument.short_name)) || (argv[pos] == "--" + argument.long_name)) {

                found = true;
                if (argument.to_perform) {
                    std::cerr << "Each argument expected no more than once, but " << argument.long_name
                              << " found twice" << std::endl;
                    throw InvalidArguments();
                }
                argument.to_perform = true;

                if (argument.arg_type == ArgType::Multiple) {
                    ++pos;
                    SubParseMultiple(argument, argc, argv, pos);
                    return;
                } else if (argument.arg_type == ArgType::Unique) {
                    ++pos;
                    SubParseUnique(argument, argc, argv, pos);
                    break;
                }
            }
        }
        if (!found) {
            std::cerr << "Invalid argument found: " << argv[pos] << std::endl;
            throw InvalidArguments();
        }
    }
}
void CLAParser::SubParseMultiple(CLAParser::ArgumentHolder& argument, int argc, char** argv, size_t& pos) {
    if (pos >= argc) {
        std::cerr << "Expected type for argument " << argument.long_name << " is: " << argument.type << std::endl;
        throw InvalidArguments();
    }
    *argument.argument_value = argv[pos];
    ++pos;
    for (; pos < argc; ++pos) {
        (*argument.argument_values).emplace_back(argv[pos]);
    }
}
void CLAParser::SubParseUnique(CLAParser::ArgumentHolder& argument, int argc, char** argv, size_t& pos) {
    if (pos >= argc) {
        std::cerr << "Expected type for argument " << argument.long_name << " is: " << argument.type << std::endl;
        throw InvalidArguments();
    }
    *argument.argument_value = argv[pos];
}
//
// Created by aris on 10/21/22.
//
#pragma once

#include <string>
#include <memory>
#include <vector>

class CLAParser {
private:
    enum class ArgType {
        Flag,
        Unique,
        Multiple,
    };

    struct ArgumentHolder {
        char short_name;
        std::string long_name;
        std::string help;
        ArgType arg_type;
        bool to_perform;
        std::string type;

        std::string* argument_value;
        std::vector<std::string>* argument_values;
    };

public:
    explicit CLAParser(const std::string program_info);

    void AddUniqueArgument(char short_name, const std::string& long_name, const std::string& type,
                           const std::string& help, std::string* store_to);

    void AddMultipleArgument(char short_name, const std::string& long_name, const std::string& type,
                             const std::string& help, std::string* store_to, std::vector<std::string>* store_vec_to);

    void AddFlag(char short_name, const std::string& long_name, const std::string& help);

    void SubParseMultiple(ArgumentHolder& argument, int argc, char** argv, size_t& pos);
    void SubParseUnique(ArgumentHolder& argument, int argc, char** argv, size_t& pos);

    void Parse(int argc, char** argv);

    bool ToPerform(const std::string& long_name) const;

    std::ostream& PrintHelpMessages(std::ostream& output) const;

private:
    std::vector<ArgumentHolder> args_;
    const std::string program_info_;
};
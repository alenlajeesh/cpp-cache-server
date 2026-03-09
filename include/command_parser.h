#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>

class CommandParser {
public:
    std::vector<std::string> parse(const std::string& input);
};

#endif

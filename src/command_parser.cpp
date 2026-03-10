#include "../include/command_parser.h"
#include <sstream>
#include <vector>

std::vector<std::string> parse_command(const std::string &input)
{
    std::vector<std::string> result;

    if (input.empty())
        return result;
    if (input[0] == '*')
    {
        std::istringstream stream(input);
        std::string line;
        std::getline(stream, line); // *num
        while (std::getline(stream, line))
        {
            if (line.empty())
                continue;
            if (line[0] == '$')
            {
                std::getline(stream, line);
                result.push_back(line);
            }
        }
    }
    else
    {
        std::istringstream iss(input);
        std::string word;
        while (iss >> word)
        {
            result.push_back(word);
        }
    }
    return result;
}

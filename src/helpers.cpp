#include "helpers.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

std::vector<std::string> split_string(const std::string &str)
{
    std::string word = "";
    std::vector<std::string> words;
    int count = 0;

    for (auto x : str)
    {
        if (count == 3)
        {
            return words;
        }
        if (x == ' ')
        {
            words.push_back(word);
            count++;
            word = "";
        }
        else
        {
            word += x;
        }
    }
    if (!word.empty())
    {
        words.push_back(word);
    }
    return words;
}

std::string extractJsonFromResponse(const std::string &response)
{
    // Find the position of the double line break
    size_t doubleLineBreakPos = response.find("\r\n\r\n");
    if (doubleLineBreakPos == std::string::npos)
    {
        return ""; // Return an empty string if the format is invalid
    }

    // Extract the JSON data starting from after the double line break
    std::string jsonData = response.substr(doubleLineBreakPos + 4);
    return jsonData;
}
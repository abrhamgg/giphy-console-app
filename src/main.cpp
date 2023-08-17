#include <iostream>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <stdio.h>
#include <string>
#include <string.h>
using namespace std;
#include <vector>
#include "helpers.h"
// #include <json.hpp>
#include <chrono>
#include <thread>
#include <json.hpp>

using namespace std;
using namespace asio;
using namespace nlohmann;
using namespace asio::ip;

void init()
{
    printf("Welcome to Giphy Console App\n");
    std::string input;

    std::cout << "Enter a Command: ";

    while (true)
    {
        std::getline(std::cin, input);
        std::vector<std::string> result = split_string(input);
        if (input == "help")
        {
            printf("Commands:\n");
            printf("next: Next Gif\n");
            printf("search <search term>: Search for a Gif\n");
            printf("cancel: Exit the program\n");
        }
        else if (input == "next")
        {
            printf("Next Gif Will be Presented\n");
        }
        else if (input == "cancel")
        {
            printf("Good Bye\n");
            break;
        }
        else if (result[0] == "search" && result.size() == 2)
        {
            printf("Searching for %s\n", result[1].c_str());
        }
        else
        {
            printf("Invalid Command\n");
        }
    }
}

class GiphyClient
{
private:
    struct Sticker
    {
        string url;
        string rank;
    };
    vector<Sticker> stickers;

    tcp::socket socket;
    io_context io;
    std::string response_json;
    int offset = 0;
    int count = 0;
    int total_count = 0;
    std::string search_criteria;

public:
    GiphyClient(io_context &io) : socket(io) {}

    void connect()
    {
        asio::ip::tcp::resolver resolver(io);
        asio::ip::tcp::endpoint endpoint = *resolver.resolve(std::string("api.giphy.com"), std::string("http")).begin();
        cout << "Connecting to the GIPHY API..." << endl;
        socket.connect(endpoint);

        if (socket.is_open())
        {
            cout << "Successfully connected to the GIPHY API" << endl;
        }
        else
        {
            cout << "Failed to connect to the GIPHY API" << endl;
        }
    }

    void parse_json()
    {
        std::string valid_json = extractJsonFromResponse(response_json);
        try
        {
            j = json::parse(valid_json);
            for (auto sticker : j["data"])
            {
                cout << sticker["url"] << endl;
            }
        }
        catch (json::parse_error &e)
        {
            std::cout << "Parse error: " << e.what() << std::endl;
            std::cout << "exception id: " << e.id << std::endl;
            std::cout << "byte position of error: " << e.byte << std::endl;
        }
    }

    void search(const string &criteria)
    {
        search_criteria = criteria;
        if (!socket.is_open())
        {
            this->connect();
        }
        asio::error_code ec;

        std::string request = "GET /v1/stickers/search?api_key=jgIRQtNyxLm2JdCk0GWDHIKVpIZWHblX&limit=50&q=" + criteria + "&offset=" + std::to_string(offset) + " HTTP/1.1\r\nHost: api.giphy.com\r\nConnection: close\r\n\r\n";

        // Send the request
        socket.write_some(asio::buffer(request.data(), request.size()), ec);

        std::string response_data;

        try
        {
            char buffer[1024];
            std::size_t bytes_transferred;

            do
            {
                bytes_transferred = socket.read_some(asio::buffer(buffer, sizeof(buffer)));
                response_data.append(buffer, bytes_transferred);
            } while (bytes_transferred > 0);
        }
        catch (std::exception &e)
        {
            std::cerr << "Error reading response: " << e.what() << std::endl;
        }

        // Parsing the JSON
        std::string json = extractJsonFromResponse(response_data);
        j = json::parse(json);
        /*
        for (auto sticker : j["data"])
        {
            cout << sticker["url"] << endl;
        }
        */
        storeStickers();
        printStickers();
        response_json = response_data;

        if (j.count("pagination"))
        {
            total_count = j["pagination"]["total_count"];
            count = j["pagination"]["count"];
        }

        socket.close();
    }

    void close()
    {
        socket.close();
    }

    int next()
    {
        offset += count;
        if (offset >= total_count)
        {
            offset = 0;
            cout << "No more Gifs to show" << endl;
            return -1;
        }
        this->search(search_criteria);
        return 1;
    }
    void storeStickers()
    {
        int len = j["data"].size();

        for (int i = 0; i < len; i++)
        {
            Sticker s;
            s.url = j["data"][i]["url"];
            s.rank = j["data"][i]["rating"];
            stickers.push_back(s);
        }
    }

    void printStickers()
    {
        for (auto sticker : stickers)
        {
            cout << sticker.url << endl;
        }
    }

private:
    json j;
};

int main()
{
    io_context io;
    GiphyClient client(io);
    client.connect();

    printf("Welcome to Giphy Console App\n");
    std::string input;

    while (true)
    {
        cout << "Enter a command: ";
        std::getline(std::cin, input);
        std::vector<std::string> result = split_string(input);
        if (input == "help")
        {
            printf("Commands:\n");
            printf("next: Next Gifs\n");
            printf("search <search term>: Search for a Gif\n");
            printf("cancel: Exit the program\n");
        }
        else if (input == "exit")
        {
            printf("Good Bye\n");
            break;
        }
        else if (result[0] == "search" && result.size() == 2)
        {
            printf("Searching for %s\n", result[1].c_str());
            client.search(result[1]);

            int flag = 0;
            std::string command;
            while (flag != -1 || command != "cancel")
            {
                cout << "$: ";
                std::getline(std::cin, command);
                if (command == "next")
                {
                    flag = client.next();
                }
                else if (command == "cancel")
                {
                    break;
                }
                else
                {
                    printf("Invalid Command\n");
                }
            }
        }
        else
        {
            printf("Invalid Command\n");
        }
    }
    return 0;
}
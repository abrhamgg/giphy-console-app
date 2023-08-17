#include <iostream>
#include <asio.hpp>
#include <json.hpp>
#include <typeinfo>
#include <thread>

using namespace std;
using namespace asio;
using namespace nlohmann;
using namespace asio::ip;

std::vector<char> vBuffer(20 * 1024);
void GrabSomeData(tcp::socket &socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()), [&](std::error_code ec, std::size_t length)
                           {
        cout << "HELLLLLLLLLLLOOOOOOOOOOO" << endl;
        if (!ec)
        {
            std::cout << "\n\nRead " << length << " bytes\n\n";

            for (size_t i = 0; i < length; i++)
            {
                std::cout << vBuffer[i];
            }

            GrabSomeData(socket);
        } 
        else
        {
            std::cout << "Error: " << ec.message() << std::endl;
} });
}

class GiphyClient
{
private:
    tcp::socket socket;
    io_context io;

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

    void search(const string &criteria)
    {
        asio::error_code ec;

        // GrabSomeData(socket);

        std::string request = "GET /v1/stickers/search?api_key=jgIRQtNyxLm2JdCk0GWDHIKVpIZWHblX&limit=100&q=" + criteria + " HTTP/1.1\r\nHost: api.giphy.com\r\nConnection: close\r\n\r\n";
        // std::string request = "GET /users HTTP/1.1\r\nHost: 127.0.0.1:3000\r\nConnection: close\r\n\r\n";

        socket.write_some(asio::buffer(request.data(), request.size()), ec);
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        //  std::this_thread::sleep_for(std::chrono::seconds(10));

        size_t bytes = socket.available();
        std::cout << "Bytes available: " << bytes << std::endl;

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

        // Now you have the complete response in 'response_data'
        std::cout << "Response: " << response_data << std::endl;

        // Now you have the complete response in 'response_data'
        cout << "READINGGGGGGGGGGGGG" << endl;
        std::cout << "Response: " << response_data << std::endl;
        // TODO: Parse the response_data string as JSON.
        /*
        if (bytes > 0)
        {



            std::vector<char> buffer(bytes);
            socket.read_some(asio::buffer(buffer.data(), buffer.size()));
            std::string response(buffer.begin(), buffer.end());
            cout << "Response: " << std::string(buffer.begin(), buffer.end()) << std::endl;
            // std::this_thread::sleep_for(std::chrono::seconds(20));

            // std::cout << "Response: " << std::string(buffer.begin(), buffer.end()) << std::endl;
            // cout << type_id(response) << endl;
            std::string jsonStr = R"(
                response
            )";
            j = json::parse(std::string(buffer.begin(), buffer.end()));

            for (auto sticker : j["data"])
            {
                cout << sticker["url"] << endl;
            }
        }
        else
        {
            std::cout << "No response" << std::endl;
        }*/
    }
    void close()
    {
        socket.close();
    }
    /*
    void next()
    {
        // Get the next page of results.
        if (j.count("pagination") && j["pagination"].count("next"))
        {
            // Send the request for the next page of results.
            socket.write("GET " + j["pagination"]["next"] + " HTTP/1.1\r\nHost: api.giphy.com\r\nConnection: close\r\n\r\n");

            // Read the response from the GIPHY API.
            string response;
            while (true)
            {
                char buf[1024];
                int n = socket.read(buf, sizeof(buf));
                if (n == 0)
                {
                    break;
                }
                response += buf;
            }

            // Parse the JSON response.
            j = json::parse(response);

            // Print the results of the search.
            for (auto sticker : j["data"])
            {
                cout << sticker["url"] << endl;
            }
        }
        else
        {
            // There is no more data to show.
            cout << "No more data" << endl;
        }
    }*/

private:
    json j;
};

int main()
{
    io_context io;
    GiphyClient client(io);

    client.connect();

    while (true)
    {
        // Get the user's command.
        cout << "Enter a command: ";
        string command;
        cin >> command;

        // Handle the command.
        if (command == "search")
        {
            // Search for stickers by criteria.
            cout << "Enter the search criteria: ";
            string criteria;
            cin >> criteria;

            client.search(criteria);
        }
        else if (command == "next")
        {
            // Get the next page of results.
            // client.next();
        }
        else if (command == "cancel")
        {
            // Cancel the ongoing search.
            client.close();
        }
        else
        {
            // Unknown command.
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}

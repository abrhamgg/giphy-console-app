Of course! Here's the entire content in Markdown format that you can copy:

```markdown
# Giphy Console App

Welcome to the Giphy Console App! This application allows you to interactively search for and view GIFs using the GIPHY API.

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/your-username/giphy-console-app.git
```

### Navigate to the Directory

```bash
cd giphy-console-app
```

### Compile and Run the Application

The project includes a Makefile that simplifies the compilation and execution process. To compile and run the application, follow these steps:

- To compile the source code and create the executable, run:

```bash
make
```

This will generate the "main" executable in the "build" directory.

- To run the compiled executable, run:

```bash
make run
```

This will execute the "main" program.

- To clean up the generated files (including the "main" executable), run:

```bash
make clean
```

### Interact with the Application

Once the application is running, you can interact with it using the command-line interface. Here are the available commands:

- `search <search term>`: Search for GIFs related to the specified search term.
    -   `next`: Display the next set of GIFs from the search results.
    - `cancel`: Stop ongoing operation to search a new gifs.
- `rank <rank>`: Count the number of stickers with the specified rank
- `exit`: Exit the program.

## Example Usage

1. Search for GIFs using a search term:

   ```
   Enter a command: search cat
   ```

   This will display GIFs related to the search term "cat."

2. Navigate to the next page of search results:

   ```
   Enter a command: next
   ```

   This will load the next page of search results if available.
   ```
    $ cancel
   ```
   This will take you to the homepage of the program.

3. Count stickers with a specific rank:

   ```
   Enter a command: rank G
   ```

   This will display the count of stickers with the rank "G."

4. Exit the program:

   ```
   Enter a command: exit
   ```

   This will exit the program.



## Requirements

- C++ compiler (supporting C++11)
- Asio library
- JSON for Modern C++
- Internet connection (required for fetching GIFs using the GIPHY API)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
```
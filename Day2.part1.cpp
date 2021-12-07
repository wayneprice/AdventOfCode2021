#include <iostream>
#include <string>


int main(int, char **)
{
    // Initialise position and depth
    int horz{0}, depth{0};

    // Read pairs of command/values
    std::string command;
    int value;
    while (std::cin >> command >> value)
    {
        // Handle commands
        if (command == "forward")
            horz += value;
        else if (command == "up")
            depth -= value;
        else if (command == "down")
            depth += value;
        else
            std::cout << "unknown command: " << command << "\n";
    }

    // Result
    std::cout << "Horizontal position: " << horz << ", Depth " << depth << "\n";
    std::cout << "Answer: " << horz * depth << "\n";

    return 0;
}

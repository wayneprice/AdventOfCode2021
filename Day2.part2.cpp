#include <iostream>
#include <string>

int main(int, char **)
{
    // Initialize values
    int horz{0}, depth{0}, aim{0};

    // Read pairs of command/values
    std::string command;
    int value;
    while (std::cin >> command >> value)
    {
        // Process commands
        if (command == "forward")
        {
            horz += value;
            depth += aim * value;
        }
        else if (command == "up")
            aim -= value;
        else if (command == "down")
            aim += value;
        else
            std::cout << "unknown command: " << command << "\n";
    }

    // Result
    std::cout << "Horizontal position: " << horz << ", Depth " << depth << "\n";
    std::cout << "Answer: " << horz * depth << "\n";

    return 0;
}

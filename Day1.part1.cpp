#include <iostream>

int main(int, char **)
{
    int count{0};

    // Read initial depth value
    int lastDepth;
    std::cin >> lastDepth;

    // Read successive depth values and count how many are greater than the previous
    // depth value
    int depth;
    while (std::cin >> depth)
    {
        if (depth > lastDepth)
            ++count;
        lastDepth = depth;
    }

    // Result
    std::cout << "Increased depths: " << count << "\n";

    return 0;
}


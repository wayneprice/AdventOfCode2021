#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>


int main(int, char **)
{
    // Read all input values. Assumes comma-separated values, but doesn't validate that
    // there's a comma between each value
    std::vector<int> input;
    int value;
    while (std::cin >> value)
    {
        input.push_back(value);
        std::cin.ignore();
    }

    // Initialise minimum fuel
    int minFuel = std::numeric_limits<int>::max();

    // Find min/max position from the input. These will form the bounds of the positions.
    int startPos = std::accumulate(input.begin(), input.end(), input[0],
                                   [](int a, int v) { return a < v ? a : v; });
    int endPos = std::accumulate(input.begin(), input.end(), input[0],
                                   [](int a, int v) { return a > v ? a : v; });

    // Search for the minima
    int minPos;
    for (auto pos = startPos; pos <= endPos; ++pos)
    {
        // Calculate the sum of fuel used based on linear position
        int fuel = std::accumulate(input.begin(), input.end(), 0,
                                   [pos](int a, int v) { return a + std::abs(v-pos); });

        // If this is less than our current fuel, store the new minima
        if (fuel < minFuel)
        {
            minFuel = fuel;
            minPos = pos;
        }
    }

    // Write output
    std::cout << "Min fuel " << minFuel << " at position " << minPos << "\n";

    return 0;
}

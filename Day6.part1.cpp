#include <iostream>
#include <vector>



int main(int, char **)
{
    // Read initial state
    std::vector<int> fish;
    int value;
    while (std::cin >> value)
    {
        fish.push_back(value);
        std::cin.ignore();
    }

    // Loop for number of days
    int numDays = 80;
    for (int day = 1; day <= numDays; ++day)
    {
        // If any counts reach zero, increase the number of fish to add on this day
        int fishToAdd{0};
        for (auto &v : fish)
        {
            if (v-- == 0)
            {
                v = 6;
                fishToAdd++;
            }
        }

        // Add new fish
        while (fishToAdd--)
        {
            fish.push_back(8);
        }
    }

    std::cout << "Total fish: " << fish.size() << "\n";

    return 0;
}


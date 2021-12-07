#include <iostream>
#include <vector>



int main(int, char **)
{
    // Setup a count of fish with a specific value
    uint64_t counts[9] = {0};

    // Read fish and increment count per fish
    int value;
    while (std::cin >> value)
    {
        counts[value]++;
        std::cin.ignore();
    }

    // Loop over all days
    int numDays = 256;
    for (int day = 1; day <= numDays; day++)
    {
        // For any fish with count 0, store with value 9 (it will be decremented
        // after processing all values)
        counts[9] = counts[0];

        // Increment count for value 7 by the number values with count 0
        counts[7] += counts[0];

        // Now shuffle all numbers down a count, essentially decrementing each fish
        // count. So the above updates to value 9 will become 8 and 7 will become 6.
        for (int i = 0; i < 9; ++i)
            counts[i] = counts[i+1];

        // Print number of fish on each day
        uint64_t total{0};
        for (int i = 0; i < 9; ++i)
            total += counts[i];
        std::cout << "Day " << day << ": Total " << total << "\n";
    }

    return 0;
}


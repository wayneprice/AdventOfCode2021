#include <iostream>

int main(int, char **)
{
    int count{0};

    // Set index to zero, and read the first three values
    int index{0};
    int depths[3] = {0};
    std::cin >> depths[0] >> depths[1] >> depths[2];

    // Store sum of values
    int sum0 = depths[0] + depths[1] + depths[2];

    // For all remaining depths, read the new depth value and compute the
    // sum of the latest two depth values and the new value. Use a rotating
    // buffer for the values
    int nextDepth;
    while (std::cin >> nextDepth)
    {
        // Check whether the sum of the latest values is greater than the
        // sum of the previous three values
        int sum1 = sum0 - depths[index % 3] + nextDepth;
        if (sum1 > sum0)
            ++count;

        // Store sum as last and overwrite in the buffer, moving the index
        // to the next value
        sum0 = sum1;
        depths[index % 3] = nextDepth;
        ++index;
    }

    // Result
    std::cout << "Increased depths: " << count << "\n";

    return 0;
}


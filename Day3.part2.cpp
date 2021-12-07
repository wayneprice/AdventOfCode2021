#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


// Count the number of 1's in the various bit positions in the set of data
std::vector<int> countOnes(const std::vector<std::string> &data)
{
    // Assumes that each element of data is the same length
    std::vector<int> count;
    count.resize(data[0].size());
    for (const auto &value : data)
    {
        for (int i = 0; i < count.size(); ++i)
        {
            count[i] += value[i] == '1';
        }
    }
    return count;
}


int main(int, char **)
{
    // Read source data
    std::vector<std::string> data;
    std::string value;
    while (std::cin >> value)
        data.push_back(value);

    // Setup oxygen as copy of data and count initial 1's
    std::vector<std::string> oxygen = data;
    std::vector<int> ones = countOnes(oxygen);
    for (int i = 0; i < ones.size(); i++)
    {
        // Copy, into tmp, values where ones are maximal
        std::vector<std::string> tmp;
        std::copy_if(oxygen.begin(), oxygen.end(), std::back_inserter(tmp),
                     [i, &ones, &oxygen](const std::string &s)
                     {
                         return s[i] == (ones[i] >= (oxygen.size() - ones[i]) ? '1' : '0');
                     });

        // Update oxygen
        oxygen = tmp;

        // Terminate when we reach a single value
        if (oxygen.size() == 1) break;

        // Update the 1's count for next time
        ones = countOnes(oxygen);
    }

    // Setup scrubber as copy of data and count initial 1's
    std::vector<std::string> scrubber = data;
    ones = countOnes(scrubber);
    for (int i = 0; i < ones.size(); i++)
    {
        // Copy, into tmp, values where 1's are minimal
        std::vector<std::string> tmp;
        std::copy_if(scrubber.begin(), scrubber.end(), std::back_inserter(tmp),
                     [i, &ones, &scrubber](const std::string &s)
                     {
                         return s[i] == (ones[i] >= (scrubber.size() - ones[i]) ? '0' : '1');
                     });

        // Update scrubber
        scrubber = tmp;

        // Terminate when we reach a single value
        if (scrubber.size() == 1) break;

        // Update the 1's count for next time
        ones = countOnes(scrubber);
    }

    // Convert binary numbers into integers
    int iOxygen = std::stoi(oxygen[0], 0, 2);
    int iScrubber = std::stoi(scrubber[0], 0, 2);

    // Result
    std::cout << iOxygen << ", " << iScrubber << " : " << iOxygen*iScrubber << "\n";
}

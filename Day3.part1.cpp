#include <iostream>
#include <string>
#include <vector>


int main(int, char **)
{
    // Read binary strings (store as strings)
    std::vector<std::string> data;
    std::string value;
    while (std::cin >> value)
        data.push_back(value);

    // Count the number of 1's in each position. Assumes all binary strings are
    // the same length
    std::vector<int> ones;
    ones.resize(data[0].size());
    for (const auto &value : data)
    {
        for (int i = 0; i < ones.size(); ++i)
        {
            ones[i] += value[i] == '1';
        }
    }

    // Calculate gamma and epsilon values.
    std::string gamma, epsilon;
    for (int i = 0; i < ones.size(); ++i)
    {
        gamma += (ones[i] > (data.size() - ones[i])) ? '1' : '0';
        epsilon += (ones[i] > (data.size() - ones[i])) ? '0' : '1';
    }

    // Convert values to integral
    int iGamma = std::stoi(gamma, 0, 2);
    int iEpsilon = std::stoi(epsilon, 0, 2);

    std::cout << iGamma << ", " << iEpsilon << " : " << iGamma*iEpsilon << "\n";
}

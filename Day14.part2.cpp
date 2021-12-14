#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <limits>


int main(int, char **)
{
    // Read polymer template
    std::string polymer;
    std::cin >> polymer;

    // read pair insertion rules
    std::map<std::string, std::string> insertionRules;
    std::string pair, dummy;
    char rule;
    while (std::cin >> pair >> dummy >> rule)
    {
        insertionRules[pair] = rule;
    }

    // Setup buckets for pairs - we don't need to perserve the original order, but just need
    // to know the pairs for the rules. We also add a cheeky ' ' to the end of the polymer
    // string so simplify counting
    std::map<std::string, long long> pairData;
    polymer.push_back(' ');
    for (int i = 0; i < polymer.size()-1; ++i)
        pairData[polymer.substr(i, 2)]++;

    // Loop over the number of steps
    int step{0};
    const int numSteps{40};

    while (step < numSteps)
    {
        // Create new pairs here, and add to this while processing a step
        std::map<std::string, long long> newPairs;

        // For each pair, lookup in the rules and create new pairs as appropriate
        for (const auto &x : pairData)
        {
            const auto rule = insertionRules.find(x.first);
            if (rule != insertionRules.end())
            {
                std::string left = { x.first[0], rule->second[0] };
                std::string right = { rule->second[0], x.first[1] };
                newPairs[left] += x.second;
                newPairs[right] += x.second;
            }
            else
            {
                newPairs[x.first] += x.second;
            }
        }

        pairData.swap(newPairs);
        ++step;
    }

    // Because we added a space to the end of the original string, in order to do the counts,
    // simple count the first element of each pair for the count of that number
    using Counts = std::map<char, long long>;
    Counts counts;
    for (const auto &c : pairData)
    {
        counts[c.first[0]] += c.second;
    }

    // Min/max
    long long min = std::numeric_limits<long long>::max();
    long long max = 0;
    for (const auto &c : counts)
    {
        if (c.second < min) min = c.second;
        if (c.second > max) max = c.second;
    }

    std::cout << "min " << (long long)min << ", max " << (long long)max << ", diff " << (long long)(max - min) << "\n";

    return 0;
}



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

    // Loop over the number of steps
    int step{0};
    const int numSteps{10};

    while (step < numSteps)
    {
        // Setup list of insertion positions. We have to make all adjustments instantaneously
        std::vector<std::tuple<int, std::string> > rulesToApply;
        for (int i = 0; i < polymer.size()-1; ++i)
        {
            std::string pair = polymer.substr(i, 2);
            const auto rule = insertionRules.find(pair);
            if (rule != insertionRules.end())
            {
                rulesToApply.emplace_back(std::make_tuple(i+1, rule->second));
            }
        }

        // Process all insertions, but do it in reverse so that we don't have to adjust
        // position indices
        while (!rulesToApply.empty())
        {
            const auto rule = rulesToApply.back();
            rulesToApply.pop_back();

            polymer.insert(std::get<0>(rule), std::get<1>(rule));
        }

        ++step;

        std::cout << step << ": " << polymer << "\n";
    }

    std::map<char, int> counts;
    for (const auto &c : polymer)
    {
        counts[c]++;
    }

    int min = std::accumulate(counts.begin(), counts.end(), std::numeric_limits<int>::max(),
         [](const int a, const std::map<char, int>::value_type &b) { return (a < b.second) ? a : b.second; });
    int max = std::accumulate(counts.begin(), counts.end(), 0,
         [](const int a, const std::map<char, int>::value_type &b) { return (a > b.second) ? a : b.second; });

    std::cout << "min " << min << ", max " << max << ", diff " << (max - min) << "\n";

    return 0;
}



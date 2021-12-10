#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <algorithm>


int main(int, char **)
{
    const std::map<char, int> penaltyScore = { {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
    const std::map<char, int> completionScore = { {')', 1}, {']', 2}, {'}', 3}, {'>', 4} };
    const std::map<char, char> pairs = { {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'} };

    std::vector<unsigned long> completionScores;
    int penalty{0};
    std::string message;
    while  (std::cin >> message)
    {
        bool valid = true;
        std::stack<char> expected;
        for (const auto c : message)
        {
            const auto p = pairs.find(c);
            if (p != pairs.end())
            {
                expected.push(p->second);
            }
            else if (c != expected.top())
            {
                //std::cout << message << ": expecting " << expected.top() << " got " << c << "\n";
                penalty += penaltyScore.at(c);
                valid = false;
                break;
            }
            else
            {
                expected.pop();
            }
        }

        if (!valid)
            continue;

        std::cout << message << ": " << expected.size() << "\n";

        unsigned long score{0};
        while (!expected.empty())
        {
            score = score*5 + completionScore.at(expected.top());
            expected.pop();
        }

        completionScores.push_back(score);
    }

    std::cout << "Penalty: " << penalty << "\n";

    std::sort(completionScores.begin(), completionScores.end());
    std::cout << "completion count " << completionScores.size() << "\n";
    for (const auto s : completionScores)
        std::cout << s << "\n";
    std::cout << "Completion: " << completionScores[completionScores.size()/2] << "\n";

    return 0;
}

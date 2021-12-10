#include <iostream>
#include <string>
#include <map>
#include <stack>


int main(int, char **)
{
    const std::map<char, int> penaltyScore = { {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
    const std::map<char, char> pairs = { {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'} };

    int penalty{0};
    std::string message;
    while  (std::cin >> message)
    {
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
                std::cout << message << ": expecting " << expected.top() << " got " << c << "\n";
                penalty += penaltyScore.at(c);
                break;
            }
            else
            {
                expected.pop();
            }
        }
    }

    std::cout << "Penalty: " << penalty << "\n";

    return 0;
}

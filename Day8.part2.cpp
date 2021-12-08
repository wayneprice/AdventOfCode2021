#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>


struct TestData
{
    std::string data[14];
};


std::vector<TestData> readData()
{
    std::vector<TestData> res;

    while (true)
    {
        TestData td;
        for (int i = 0; i < 10; ++i)
        {
            std::cin >> td.data[i];
            std::sort(td.data[i].begin(), td.data[i].end());
        }

        std::string dummy;
        std::cin >> dummy;

        for (int i = 0; i < 4; i++)
        {
            std::cin >> td.data[i+10];
            std::sort(td.data[i+10].begin(), td.data[i+10].end());
        }

        if (std::cin.eof())
            break;

        res.emplace_back(td);
    }

    return res;
}


std::string commonSegments(const std::string &s1, const std::string &s2)
{
    std::string res;
    for (const auto c : s1)
    {
        if (s2.find(c) != std::string::npos)
            res += c;
    }
    return res;
}


int main(int, char **)
{
    int total{0};
    const auto testData = readData();

    // Find all unique combinations in the data
    for (const auto &td : testData)
    {
        std::map<std::string, int> segmentMap;
        for (int i = 0; i < 14; ++i)
            segmentMap[td.data[i]] = -1;

        // Map simple entries
        std::map<int, std::string> digitMap;
        for (auto &s : segmentMap)
        {
            const auto len = s.first.size();
            if (len == 2)
            {
                s.second = 1;
                digitMap[s.second] = s.first;
            }
            else if (len == 3)
            {
                s.second = 7;
                digitMap[s.second] = s.first;
            }
            else if (len == 4)
            {
                s.second = 4;
                digitMap[s.second] = s.first;
            }
            else if (len == 7)
            {
                s.second = 8;
                digitMap[s.second] = s.first;
            }
        }

        // Now map others - digits 0, 6, 9 have six segments, and digits 2, 3, 5 have five
        for (auto &s : segmentMap)
        {
            const auto len = s.first.size();

            // For len 6, digits are either 0, 6 or 9
            if (len == 6)
            {
                const auto common4 = commonSegments(s.first, digitMap[4]);
                const auto common7 = commonSegments(s.first, digitMap[7]);

                // Digit 9 has four segments common with a 4
                if (common4.size() == 4)
                {
                    s.second = 9;
                    digitMap[s.second] = s.first;
                }

                // Digit 6 has two segments common with a 4
                else if (common7.size() == 2)
                {
                    s.second = 6;
                    digitMap[s.second] = s.first;
                }

                // Digit 0 has three segments common with a 7
                else if (common7.size() == 3)
                {
                    s.second = 0;
                    digitMap[s.second] = s.first;
                }
            }

            // For len 5, digits are either 2, 3 or 5
            else if (len == 5)
            {
                // Digit 3 has two segments in common with a 1
                const auto common1 = commonSegments(s.first, digitMap[1]);
                const auto common4 = commonSegments(s.first, digitMap[4]);
                if (common1.size() == 2)
                {
                    s.second = 3;
                    digitMap[s.second] = s.first;
                }

                // Digit 2 has two segments in common with a 4
                else if (common4.size() == 2)
                {
                    s.second = 2;
                    digitMap[s.second] = s.first;
                }

                // Digit 5 has three segments in common with a 4
                else if (common4.size() == 3)
                {
                    s.second = 5;
                    digitMap[s.second] = s.first;
                }
            }
        }

        // Count, and sum, the output data
        {
            int value = segmentMap[td.data[10]]*1000 +
                        segmentMap[td.data[11]]*100 +
                        segmentMap[td.data[12]]*10 +
                        segmentMap[td.data[13]];
            total += value;
        }
    }

    std::cout << "Sum of outputs " << total << "\n";

    return 0;
}


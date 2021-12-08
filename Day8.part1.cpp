#include <iostream>
#include <vector>
#include <string>


struct TestData
{
    std::string input[10];
    std::string output[10];
};


std::vector<TestData> readData()
{
    std::vector<TestData> res;

    while (true)
    {
        TestData data;
        for (int i = 0; i < 10; ++i)
        {
            std::cin >> data.input[i];
        }

        std::string dummy;
        std::cin >> dummy;

        for (int i = 0; i < 4; i++)
        {
            std::cin >> data.output[i];
        }

        if (std::cin.eof())
            break;
        res.emplace_back(data);
    }

    return res;
}



int main(int, char **)
{
    const auto testData = readData();

    int count1478{0};
    for (const auto &td : testData)
    {
        for (int i = 0; i < 4; i++)
        {
            const auto len = td.output[i].size();
            if (len == 2 || len == 3 || len == 4 || len == 7)
                ++count1478;
        }
    }

    std::cout << "1,4,7,8 occur " << count1478 << " times\n";

    return 0;
}


#include <iostream>
#include <vector>


struct Point
{
    int x, y;
};


int main(int, char **)
{
    int data[10][10];
    for (int y = 0; y < 10; ++y)
    {
        std::string line;
        std::cin >> line;
        for (int x = 0; x < 10; ++x)
        {
            data[y][x] = int(line[x] - '0');
        }
    }

    int flashCount{0};
    int numSteps{0};
    while (true)
    {
        ++numSteps;
        std::vector<Point> flashes;
        for (int y = 0; y < 10; ++y)
        {
            for (int x = 0; x < 10; ++x)
            {
                if (++data[y][x] == 10)
                {
                    flashes.push_back(Point{x, y});
                }
            }
        }

        int flashNum{0};
        Point adjacency[8] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };
        while (flashNum < flashes.size())
        {
            int x = flashes[flashNum].x;
            int y = flashes[flashNum].y;
            for (int i = 0; i < 8; i++)
            {
                int ax = x + adjacency[i].x;
                int ay = y + adjacency[i].y;
                if (ax >= 0 && ay >= 0 && ax < 10 && ay < 10)
                {
                    if (++data[ay][ax] == 10)
                    {
                        flashes.push_back(Point{ax, ay});
                    }
                }
            }
            ++flashNum;
        }

        for (int y = 0; y < 10; ++y)
        {
            for (int x = 0; x < 10; ++x)
            {
                if (data[y][x] >= 10)
                {
                    data[y][x] = 0;
                }
            }
        }

        std::cout << "After step " << numSteps << "\n";
        for (int y = 0; y < 10; ++y)
        {
            for (int x = 0; x < 10; ++x)
            {
                std::cout << data[y][x];
            }
            std::cout << "\n";
        }
        flashCount += flashNum;
        std::cout << "Flashes " << flashCount << "\n";
        std::cout << "\n";

        if (flashNum == 100)
        {
            std::cout << "### SIMULTANEOUS FLASH ###\n";
            break;
        }
    }

    return 0;
}


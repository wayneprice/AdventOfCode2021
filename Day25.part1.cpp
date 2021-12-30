#include <iostream>
#include <vector>
#include <string>



using Grid = std::vector<std::string>;


int processStep(Grid &grid)
{
    int count = 0;

    // Process horizontal
    {
        Grid newGrid = grid;
        for (int y = 0; y < grid.size(); ++y)
        {
            for (int x = 0; x < grid[0].size(); ++x)
            {
                if (grid[y][x] == '>')
                {
                    int xr = (x+1) % grid[y].size();
                    if (grid[y][xr] == '.')
                    {
                        newGrid[y][xr] = grid[y][x];
                        newGrid[y][x] = '.';
                        ++count;
                    }
                }
            }
        }

        std::swap(grid, newGrid);
    }

    // Process vertical
    {
        Grid newGrid = grid;
        for (int x = 0; x < grid[0].size(); ++x)
        {
            for (int y = 0; y < grid.size(); ++y)
            {
                if (grid[y][x] == 'v')
                {
                    int yd = (y+1) % grid.size();
                    if (grid[yd][x] == '.')
                    {
                        newGrid[yd][x] = grid[y][x];
                        newGrid[y][x] = '.';
                        ++count;
                    }
                }
            }
        }
        std::swap(grid, newGrid);
    }

    return count;
}



int main(int, char **)
{
    // Read input

    Grid grid;
    std::string line;
    while (std::cin >> line)
        grid.emplace_back(line);

    // Process each step
    int steps{0};
    while (true)
    {
        int changes = processStep(grid);
        ++steps;

        if (changes == 0)
            break;
    }

    std::cout << "Steps to stable state: " << steps << "\n";

    return 0;
}

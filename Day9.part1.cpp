#include <iostream>
#include <string>
#include <vector>


class HeightMap
{
public:
    void readHeightMap();

    int operator()(int x, int y) const;
    int width() const       { return width_; }
    int height() const      { return height_; }

private:
    std::vector<std::string> map_;
    int width_, height_;
};


void HeightMap::readHeightMap()
{
    std::string row;
    while (std::cin >> row)
        map_.push_back(row);

    width_ = map_[0].size();
    height_ = map_.size();
}


int HeightMap::operator()(int x, int y) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return 9;

    return int(map_[y][x] - '0');
}



int main(int, char **)
{
    HeightMap heightMap;
    heightMap.readHeightMap();

    int riskSum{0};
    for (int y = 0; y < heightMap.height(); ++y)
    {
        for (int x = 0; x < heightMap.width(); ++x)
        {
            if ((heightMap(x, y) < heightMap(x-1, y)) &&
                (heightMap(x, y) < heightMap(x, y-1)) &&
                (heightMap(x, y) < heightMap(x+1, y)) &&
                (heightMap(x, y) < heightMap(x, y+1)))
            {
                riskSum += heightMap(x, y) + 1;
            }
        }
    }

    std::cout << "Sum of Risk Levels = " << riskSum << "\n";

    return 0;
}


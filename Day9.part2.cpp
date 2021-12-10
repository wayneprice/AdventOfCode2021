#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>


struct Point
{
    int x, y;
};

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


class Array2D
{
public:
    Array2D(int width, int height, int outOfBounds = 9)
    : width_(width), height_(height), outOfBounds_(outOfBounds)
    {
        data_.resize(width_*height_);
    }

    int operator()(int x, int y) const
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return outOfBounds_;
        return data_[y*width_ + x];
    }

    void set(int x, int y, int value)
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return;
        data_[y*width_ + x] = value;
    }

    int width() const   { return width_; }
    int height() const  { return height_; }

private:
    int width_, height_;
    int outOfBounds_;
    std::vector<int> data_;
};


int main(int, char **)
{
    HeightMap heightMap;
    heightMap.readHeightMap();

    std::vector<Point> minima;
    for (int y = 0; y < heightMap.height(); ++y)
    {
        for (int x = 0; x < heightMap.width(); ++x)
        {
            if ((heightMap(x, y) < heightMap(x-1, y)) &&
                (heightMap(x, y) < heightMap(x, y-1)) &&
                (heightMap(x, y) < heightMap(x+1, y)) &&
                (heightMap(x, y) < heightMap(x, y+1)))
            {
                minima.emplace_back(Point{x, y});
            }
        }
    }

    // Results
    struct Basin
    {
        Point p;
        int size{0};
    };
    std::vector<Basin> basins;

    // Setup empty array of filled points
    Array2D counted(heightMap.width(), heightMap.height());

    // Label any depth values of 9 as processed
    for (int y = 0; y < heightMap.height(); ++y)
    {
        for (int x = 0; x < heightMap.width(); ++x)
        {
            if (heightMap(x, y) == 9)
            {
                counted.set(x, y, 1);
            }
        }
    }

    // Essentially, we now implement a fill algorithm!aAnd count the items
    for (const auto &p : minima)
    {
        Basin basin;
        basin.p = p;

        // Structure used to hold fill spans
        struct Span
        {
            int xmin, xmax;
            int y;
        };
        std::stack<Span> spanStack;

        // Find initial span surrounding this start point
        int xmin = p.x;
        while (xmin >= 0 && !counted(xmin, p.y))
            --xmin;
        int xmax = p.x;
        while (xmax < counted.width() && !counted(xmax, p.y))
            ++xmax;
        spanStack.emplace(Span{xmin+1, xmax-1, p.y});

        // For all spans in the stack, fill the points (so we don't process again), and
        // look for more spans above/below this span, which satify adjacency rules
        while (!spanStack.empty())
        {
            Span s = spanStack.top();
            spanStack.pop();

            // Mark cells in this span as processed and counted
            for (int x = s.xmin; x <= s.xmax; ++x)
            {
                if (!counted(x, s.y))
                {
                    counted.set(x, s.y, 1);
                    basin.size += 1;
                }
            }

            // Now, we need to look for rows above and rows below which are adjoining this
            // span, and which haven't been processed or counted yet.
            for (int yoff = -1; yoff <= 1; yoff+=2)
            {
                if (s.y + yoff >= 0 && s.y + yoff < counted.height())
                {
                    // Start immediately above/below the start position, and search left
                    // until we find a boundary
                    int y = s.y + yoff;
                    int x = s.xmin;
                    while (x >= 0 && !counted(x, y))
                        --x;

                    // Loop until the end of the current span
                    while (x <= s.xmax)
                    {
                        // Here, we have a position which is a boundary point. Find the first non-boundary
                        // point which is within the current span
                        while (x < counted.width() && x <= s.xmax && counted(x, y))
                            ++x;

                        // Only add this as a span if we are adjacent to the current span
                        if (x <= s.xmax)
                        {
                            // If we have a valid start-of-span, look for the end of the span, and add
                            // to the span list for more processing
                            int x0 = x++;
                            while (x <= counted.width() && !counted(x, y))
                                ++x;

                            if (x-1 >= x0)
                            {
                                spanStack.emplace(Span{x0, x-1, y});
                            }
                        }
                    }
                }
            }
        }

        basins.emplace_back(basin);
    }

    std::sort(basins.begin(), basins.end(), [](const Basin &a, const Basin &b) { return a.size > b.size; });
    for (const auto &basin : basins)
    {
        std::cout << basin.p.x << ", " << basin.p.y << ": " << basin.size << "\n";
    }

    std::cout << "Largest basins: " << basins[0].size << ", " << basins[1].size << ", " << basins[2].size << "\n";
    std::cout << "Result = " << basins[0].size * basins[1].size * basins[2].size << "\n";
    return 0;
}


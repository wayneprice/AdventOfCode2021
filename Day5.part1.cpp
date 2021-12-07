#include <iostream>
#include <vector>
#include <algorithm>


struct Point
{
    int x, y;
};

struct Line
{
    Point p1, p2;
};

// Read line data
std::vector<Line> readLines(std::istream &in)
{
    std::vector<Line> lines;

    while (true)
    {
        // Read initial (x, y)
        int x, y;
        char c;
        if (!(in >> x >> c >> y))
            break;
        Point p1{x, y};

        // Skip over "->" separator (no validation)
        std::string dummy;
        in >> dummy;

        // Read second pair (x, y)
        if (!(in >> x >> c >> y))
            break;
        Point p2{x, y};

        // Add line
        lines.emplace_back(Line{p1, p2});
    }

    return lines;
}


class Grid
{
public:
    Grid(int xmin, int xmax, int ymin, int ymax)
    : xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax)
    , width_(xmax_ - xmin_ + 1), height_(ymax_ - ymin_ + 1)
    {
        cells_.resize(width_ * height_);
    }

    int &cell(int x, int y)
    {
        // No validation on bounds here
        return cells_[(x - xmin_) + (y - ymin_)*width_];
    }

    int overlapping() const
    {
        int total{0};
        for (const auto count : cells_)
        {
            total += (count > 1);
        }
        return total;
    }

private:
    int xmin_, xmax_, ymin_, ymax_;
    int width_, height_;
    std::vector<int> cells_;
};



int main(int, char **)
{
    // Read all lines
    const auto lines = readLines(std::cin);

    // Calculate extents of all lines
    int xmin, xmax, ymin, ymax;
    xmin = std::min(lines[0].p1.x, lines[0].p2.x);
    ymin = std::min(lines[0].p1.y, lines[0].p2.y);
    xmax = std::max(lines[0].p1.x, lines[0].p2.x);
    ymax = std::max(lines[0].p1.y, lines[0].p2.y);
    for (const auto &line : lines)
    {
        xmin = std::min(xmin, std::min(line.p1.x, line.p2.x));
        ymin = std::min(ymin, std::min(line.p1.y, line.p2.y));
        xmax = std::max(xmax, std::max(line.p1.x, line.p2.x));
        ymax = std::max(ymax, std::max(line.p1.y, line.p2.y));
    }

    Grid grid(xmin, xmax, ymin, ymax);
    for (const auto &line : lines)
    {
        if (line.p1.x == line.p2.x)
        {
            int x = line.p1.x;
            if (line.p1.y < line.p2.y)
                for (int y = line.p1.y; y <= line.p2.y; ++y)
                    ++grid.cell(x, y);
            else
                for (int y = line.p1.y; y >= line.p2.y; --y)
                    ++grid.cell(x, y);
        }
        else if (line.p1.y == line.p2.y)
        {
            int y = line.p1.y;
            if (line.p1.x < line.p2.x)
                for (int x = line.p1.x; x <= line.p2.x; ++x)
                    ++grid.cell(x, y);
            else
                for (int x = line.p1.x; x >= line.p2.x; --x)
                    ++grid.cell(x, y);
        }
    }

    std::cout << "overlapping " << grid.overlapping() << "\n";

    return 0;
}



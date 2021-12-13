#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>


struct Point
{
    int x, y;
};

struct Fold
{
    char direction;
    int coord;
};

class Array2D
{
public:
    Array2D(int width, int height)
    : width_(width), height_(height)
    {
        data_.resize(width_*height_);
    }

    char &valueAt(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width_ || y >= height_)
            throw std::runtime_error("coordinate out-of-range");
        return data_[x + y*width_];
    }

    int width() const	{ return width_; }
    int height() const	{ return height_; }

    int count(int value)
    {
        int result{0};
        for (const auto &v : data_)
            result += (v == value);
        return result;
    }

    Array2D fold(char direction, int coord);

private:
    int width_, height_;
    std::vector<char> data_;
};


Array2D Array2D::fold(char direction, int coord)
{
    int newWidth = (direction == 'x') ? coord : width_;
    int newHeight = (direction == 'y') ? coord : height_;

    Array2D foldedData(newWidth, newHeight);
    for (int y = 0; y < height_; ++y)
    {
        int foldedY = (direction == 'y' && y >= coord) ? y-(y-coord)*2 : y;
        for (int x = 0; x < width_; ++x)
        {
            if (valueAt(x, y))
            {
                int foldedX = (direction == 'x' && x >= coord) ? x-(x-coord)*2 : x;
                foldedData.valueAt(foldedX, foldedY) = valueAt(x, y);
            }
        }
    }

    return foldedData;
}



int main(int, char **)
{
    // Read points from input. No validation of format is performed
    std::vector<Point> points;
    std::string line;
    int xmax{0}, ymax{0};
    while (true)
    {
        std::getline(std::cin, line);
        if (line.empty())
            break;

        std::istringstream iss(line);
        char c;
        Point p;
        iss >> p.x >> c >> p.y;
        xmax = std::max(xmax, p.x);
        ymax = std::max(ymax, p.y);
        points.emplace_back(p);
    }

    // Read instructions
    std::vector<Fold> folds;
    while (true)
    {
        std::getline(std::cin, line);
        if (line.empty())
            break;

        std::istringstream iss(line);
        std::string dummy;
        char c;
        Fold fold;
        // Format is "fold along x=123" or "fold along y=567"
        iss >> dummy >> dummy >> fold.direction >> c >> fold.coord;
        folds.emplace_back(fold);
    }

    // Write points to a grid
    Array2D grid(xmax+1, ymax+1);
    for (const auto &p : points)
    {
        grid.valueAt(p.x, p.y) = 1;
    }

    // Fold first instruction
    Array2D folded = grid.fold(folds[0].direction, folds[0].coord);
    std::cout << "Points " << folded.count(1) << "\n";

    return 0;
}

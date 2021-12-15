#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


struct Point
{
    int x, y;

    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
};


class Array2D
{
public:
    Array2D(int width, int height)
    : width_(width), height_(height)
    {
        data_.resize(width_*height_);
    }

    int get(int x, int y) const
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return -1;
        return data_[y*width_ + x].value_;
    }

    int getVisited(int x, int y) const
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return -1;
        return data_[y*width_ + x].visited_;
    }

    void set(int x, int y, int value)
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return;
        data_[y*width_ + x].value_ = value;
    }

    void setVisited(int x, int y, int value)
    {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return;
        data_[y*width_ + x].visited_ = value;
    }

    int width() const   { return width_; }
    int height() const  { return height_; }

private:
    int width_, height_;

    struct Data
    {
        int value_;
        int visited_;
    };
    std::vector<Data> data_;
};


struct TraversalPoint
{
    Point p;
    int score;
    std::vector<Point> path;

    bool operator<(const TraversalPoint &other) const { return score < other.score; }
};


int main(int, char **)
{
    // Read input data
    std::vector<std::string> data;
    std::string line;
    while (std::cin >> line)
        data.push_back(line);

    // Convert into 2D array
    Array2D grid(data.size(), data[0].size());
    for (int y = 0; y < data.size(); ++y)
    {
        for (int x = 0; x < data[0].size(); ++x)
        {
            grid.set(x, y, int(data[y][x] - '0'));
        }
    }

    const Point startPoint{0, 0};
    const Point endPoint{grid.width()-1, grid.height()-1};

    // Create a vertor of points in the grid. Each point will have a score and position.
    // We look for all options from the current point and store these in a sorted order
    // in the list. 
    std::multiset<TraversalPoint> traversalPoints;
    traversalPoints.emplace(TraversalPoint{startPoint, 0, {startPoint}});

    while (traversalPoints.begin()->p.x != endPoint.x || traversalPoints.begin()->p.y != endPoint.y)
    {
        // Take topmost (lowest cost) point from the stack
        auto t = traversalPoints.begin();
        const Point p = t->p;
        const int score = t->score;
        const auto path = t->path;
        grid.setVisited(p.x, p.y, true);
        traversalPoints.erase(t);

        // Add new points corresponding to unvisited cells in the grid, adjacent to this
        // point, with appropriate score based on value in that grid location

        if (p.x > 0)
        {
            int x = p.x-1;
            int y = p.y;
            if (!grid.getVisited(x, y))
            {
                TraversalPoint newPoint{Point{x, y}, score + grid.get(x, y), path};
                newPoint.path.push_back(Point{x, y});
                traversalPoints.insert(newPoint);
                grid.setVisited(x, y, true);
            }
        }
        if (p.x < grid.width()-1)
        {
            int x = p.x+1;
            int y = p.y;
            if (!grid.getVisited(x, y))
            {
                TraversalPoint newPoint{Point{x, y}, score + grid.get(x, y), path};
                newPoint.path.push_back(Point{x, y});
                traversalPoints.insert(newPoint);
                grid.setVisited(x, y, true);
            }
        }
        if (p.y > 0)
        {
            int x = p.x;
            int y = p.y-1;
            if (!grid.getVisited(x, y))
            {
                TraversalPoint newPoint{Point{x, y}, score + grid.get(x, y), path};
                newPoint.path.push_back(Point{x, y});
                traversalPoints.insert(newPoint);
                grid.setVisited(x, y, true);
            }
        }
        if (p.y < grid.height()-1)
        {
            int x = p.x;
            int y = p.y+1;
            if (!grid.getVisited(x, y))
            {
                TraversalPoint newPoint{Point{x, y}, score + grid.get(x, y), path};
                newPoint.path.push_back(Point{x, y});
                traversalPoints.insert(newPoint);
                grid.setVisited(x, y, true);
            }
        }
    }

    const auto bestRoute = *traversalPoints.begin();
    std::cout << "Risk " << bestRoute.score << "\n";

    for (int y = 0; y < grid.height(); ++y)
    {
        for (int x = 0; x < grid.width(); ++x)
        {
            if (std::find(bestRoute.path.begin(), bestRoute.path.end(), Point{x,y}) != bestRoute.path.end())
            {
                std::cout << "\033[1m" << grid.get(x, y) << "\033[0m";
            }
            else
            {
                std::cout << grid.get(x, y);
            }
        }
        std::cout << "\n";
    }


    return 0;
}



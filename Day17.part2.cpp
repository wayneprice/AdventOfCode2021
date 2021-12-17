#include <iostream>
#include <cmath>


struct TargetArea
{
    int xmin, xmax;
    int ymin, ymax;

    bool inside(int x, int y) const
    {
        return x >= xmin && x <= xmax && y >= ymin && y <= ymax;
    }
};


int maximumHeight(int yvel)
{
    if (yvel < 0)
        return 0;
    return (yvel*(yvel+1)) / 2;
}


bool reachesTarget(int xvel, int yvel, const TargetArea &targetArea)
{
    int x{0}, y{0};

    while (x <= targetArea.xmax && y >= targetArea.ymin)
    {
        if (targetArea.inside(x, y)) return true;

        x += xvel;
        y += yvel;
        xvel = std::max(0, xvel-1);
        yvel -= 1;
    }

    return false;
}



int main(int, char **)
{
    // Read target area input. Probably a better way with regex
    TargetArea targetArea;

    char c;
    std::string dummy;
    std::cin >> dummy >> dummy;
    std::cin >> c >> c >> targetArea.xmin >> c >> c >> targetArea.xmax >> dummy;
    std::cin >> c >> c >> targetArea.ymin >> c >> c >> targetArea.ymax;

    // Calculate the minimum X velocity required. The x velocity reduces by one each step
    // until it hits zero. This will be the minimum bound for the initial x velocity
    int minXVelocity = int((sqrt(1 + 8*targetArea.xmin)-1.)/2);

    // The maximum X velocity would be such that after one step, we have already missed
    // the target area
    int maxXVelocity = targetArea.xmax;

    // The minimum bound for the Y velocity is such that the first step moves the probe
    // downwards to the bottom of the target area
    int minYVelocity = targetArea.ymin;

    // The maximum bound for Y velocity is such that one step from zero in the downward
    // direction would would pass the target area completely. As the 'curve' is symmetrical
    // the velocity at zero will be the negative of the upward initial velocity
    int maxYVelocity = -targetArea.ymin;

    // Find count of velocities which will hit the target
    int targetVelocities{0};

    for (int xVel = minXVelocity; xVel <= maxXVelocity; ++xVel)
    {
        for (int yVel = minYVelocity; yVel <= maxYVelocity; ++yVel)
        {
            if (reachesTarget(xVel, yVel, targetArea))
            {
                ++targetVelocities;   
            }
        }
    }

    std::cout << "Count of velocities which his target = " << targetVelocities << "\n";

    return 0;
}


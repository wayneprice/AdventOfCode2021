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

    // The maximum bound for Y velocity is such that one step from zero in the downward
    // direction would would pass the target area completely. As the 'curve' is symmetrical
    // the velocity at zero will be the negative of the upward initial velocity
    int maxYVelocity = -targetArea.ymin;

    // Find trajectories looking for the maximum height. There's probably no point searching
    // -ve y velocities as the maximum height for these will be the starting height 0. When
    // we find a trajectory which lands in the target area, we can stop looking for lower
    // y velocities as these won't beat that height, so reducing the number of trajectories
    // we need to check
    int bestYVel = 0;
    for (int xVel = minXVelocity; xVel <= maxXVelocity; ++xVel)
    {
        for (int yVel = maxYVelocity; yVel > bestYVel; --yVel)
        {
            if (reachesTarget(xVel, yVel, targetArea))
            {
                bestYVel = yVel;
                break;
            }
        }
    }

    std::cout << "Best Y velocity = " << bestYVel << ", height " << maximumHeight(bestYVel) << "\n";

    return 0;
}


#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <tuple>


enum class State { On, Off };

struct Instruction
{
    State state;
    int xmin, xmax;
    int ymin, ymax;
    int zmin, zmax;
};


struct Cuboid
{
    int xmin, xmax;
    int ymin, ymax;
    int zmin, zmax;
    int sign{1};

    bool intersects(const Cuboid &other) const
    {
        return (xmin <= other.xmax && xmax >= other.xmin &&
                ymin <= other.ymax && ymax >= other.ymin &&
                zmin <= other.zmax && zmax >= other.zmin);
    }

    Cuboid intersection(const Cuboid &other) const
    {
        return Cuboid{std::max(xmin, other.xmin), std::min(xmax, other.xmax),
                      std::max(ymin, other.ymin), std::min(ymax, other.ymax),
                      std::max(zmin, other.zmin), std::min(zmax, other.zmax)};
    }

    friend std::ostream &operator<<(std::ostream &os, Cuboid c)
    {
        os << "Cuboid(" << c.xmin << ", " << c.xmax << ", " << c.ymin << ", " << c.ymax << ", " << c.zmin << ", " << c.zmax << ") sign " << c.sign;
        return os;
    }
};



std::vector<Instruction> readInstructions(std::istream &in)
{
    std::vector<Instruction> res;

    while (true)
    {
        std::string s;
        if (!(in >> s))
            break;

        Instruction instruction;
        instruction.state = (s == "on" ? State::On : State::Off);

        char c;
        in >> c >> c >> instruction.xmin >> c >> c >> instruction.xmax >> c;
        in >> c >> c >> instruction.ymin >> c >> c >> instruction.ymax >> c;
        in >> c >> c >> instruction.zmin >> c >> c >> instruction.zmax;

        if (instruction.xmin > instruction.xmax) std::swap(instruction.xmin, instruction.xmax);
        if (instruction.ymin > instruction.ymax) std::swap(instruction.ymin, instruction.ymax);
        if (instruction.zmin > instruction.zmax) std::swap(instruction.zmin, instruction.zmax);

        res.emplace_back(instruction);
    }

    return res;
}



int main(int, char **)
{
    auto instructions = readInstructions(std::cin);

    std::vector<Cuboid> cuboids;

    for (const auto &instruction : instructions)
    {
        Cuboid newCuboid{instruction.xmin, instruction.xmax,
                         instruction.ymin, instruction.ymax,
                         instruction.zmin, instruction.zmax};

        std::vector<Cuboid> newAdditions;
        if (instruction.state == State::On)
            newAdditions.push_back(newCuboid);

        for (const auto &c : cuboids)
        {
            if (c.intersects(newCuboid))
            {
                Cuboid intersection = c.intersection(newCuboid);
                intersection.sign = -c.sign;
                newAdditions.emplace_back(intersection);
            }
        }

        std::copy(newAdditions.begin(), newAdditions.end(), std::back_inserter(cuboids));
    }

    long long count{0};
    for (const auto &c : cuboids)
    {
        count += static_cast<long long>(c.sign) * (c.xmax - c.xmin + 1) * (c.ymax - c.ymin + 1) * (c.zmax - c.zmin + 1);
    }
    std::cout << "Count of set values: " << count << "\n";

    return 0;
}


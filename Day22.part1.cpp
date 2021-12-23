#include <iostream>
#include <vector>


enum class State { On, Off };

struct Instruction
{
    State state;
    int xmin, xmax;
    int ymin, ymax;
    int zmin, zmax;
};


class Grid3D
{
public:
    Grid3D(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax)
    : xmin_(xmin), xmax_(xmax), ymin_(ymin), ymax_(ymax), zmin_(zmin), zmax_(zmax)
    {
        xsize_ = xmax_ - xmin_ + 1;
        ysize_ = ymax_ - ymin_ + 1;
        zsize_ = zmax_ - zmin_ + 1;
        data_.resize(xsize_ * ysize_ * zsize_);
        for (int i = 0; i < xsize_*ysize_*zsize_; ++i)
            data_[i] = false;
    }

    void set(int x, int y, int z, bool value)
    {
        if (x >= xmin_ && x <= xmax_ && y >= ymin_ && y <= ymax_ && z >= zmin_ && z <= zmax_)
            data_[(x-xmin_) + xsize_*((y-ymin_) + ysize_*(z-zmin_))] = value;
    }

    bool get(int x, int y, int z) const
    {
        if (x >= xmin_ && x <= xmax_ && y >= ymin_ && y <= ymax_ && z >= zmin_ && z <= zmax_)
            return data_[(x-xmin_) + xsize_*((y-ymin_) + ysize_*(z-zmin_))];
        return false;
    }

    void setCuboid(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax, bool value)
    {
        if (zmin < zmin_ || zmax > zmax_ || ymin < ymin_ || ymax > ymax_ || xmin < xmin_ || xmax > xmax_)
            return;

        for (int z = zmin; z <= zmax; ++z)
        {
            int zoff = (z-zmin_)*ysize_*xsize_;
            for (int y = ymin; y <= ymax; ++y)
            {
                int yoff = (y-ymin_)*xsize_;
                for (int x = xmin; x <= xmax; ++x)
                {
                    data_[(x-xmin_) + yoff + zoff] = value;
                }
            }
        }
    }

    int countValues() const
    {
        int res{0};
        for (int i = 0; i < data_.size(); ++i)
            res += (data_[i] == true);
        return res;
    }

private:
    int xmin_, xmax_;
    int ymin_, ymax_;
    int zmin_, zmax_;
    int xsize_, ysize_, zsize_;
    std::vector<bool> data_;
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

        res.emplace_back(instruction);
    }

    return res;
}



int main(int, char **)
{
    auto instructions = readInstructions(std::cin);
    
    Grid3D grid(-50, 50, -50, 50, -50, 50);

    for (const auto &instruction : instructions)
    {
        grid.setCuboid(instruction.xmin, instruction.xmax,
                       instruction.ymin, instruction.ymax,
                       instruction.zmin, instruction.zmax, instruction.state == State::On);
    }

    std::cout << "Count of set values: " << grid.countValues() << "\n";

    return 0;
}


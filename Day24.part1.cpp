#include <iostream>
#include <sstream>
#include <vector>
#include <variant>
#include <stdexcept>
#include <tuple>
#include <map>
#include <algorithm>
#include <stack>
#include <set>


struct Instruction
{
    std::string opcode;
    std::string x;
    std::variant<std::string, long long> y;

    friend std::ostream &operator<<(std::ostream &os, const Instruction &inst)
    {
        os << inst.opcode << " " << inst.x;
        if (inst.opcode != "inp")
        {
            if (std::holds_alternative<std::string>(inst.y))
                os << " " << std::get<std::string>(inst.y);
            else
                os << " " << std::get<long long>(inst.y);
        }
        return os;
    }
};


std::vector<Instruction> readInstructions(std::istream &in)
{
    std::vector<Instruction> instructions;

    while (true)
    {
        Instruction instruction;
        if (!(in >> instruction.opcode))
            break;

        in >> instruction.x;
        if (instruction.opcode != "inp")
        {
            std::string s;
            in >> s;
            try
            {
                instruction.y = std::stoll(s);
            }
            catch (...)
            {
                instruction.y = s.substr(0);
            }
        }

        instructions.emplace_back(instruction);
    }

    return instructions;
}


class ALU
{
public:
    void processInstruction(const Instruction &inst, std::istream &in);

    long long value(const std::variant<std::string, long long> &x)
    {
        if (std::holds_alternative<std::string>(x))
            return reg(std::get<std::string>(x));
        else
            return std::get<long long>(x);
    }

    long long &reg(std::string r)
    {
        return r == "x" ? x : r == "y" ? y : r == "z" ? z : w;
    }

private:
    long long x{0}, y{0}, z{0}, w{0};
};



void ALU::processInstruction(const Instruction &inst, std::istream &in)
{
    if (inst.opcode == "inp")
    {
        char c;
        in >> c;
        reg(inst.x) = int(c - '0');
    }
    else if (inst.opcode == "add")
    {
        reg(inst.x) = reg(inst.x) + value(inst.y);
    }
    else if (inst.opcode == "mul")
    {
        reg(inst.x) = reg(inst.x) * value(inst.y);
    }
    else if (inst.opcode == "div")
    {
        long long denom = value(inst.y);
        if (denom == 0)
            throw std::runtime_error("divide by zero");
        reg(inst.x) = reg(inst.x) / denom;
    }
    else if (inst.opcode == "mod")
    {
        long long num = reg(inst.x);
        long long denom = value(inst.y);
        if (num < 0 || denom <= 0)
            throw std::runtime_error("invalid modulo values");
        reg(inst.x) = num % denom;
    }
    else if (inst.opcode == "eql")
    {
        reg(inst.x) = (reg(inst.x) == value(inst.y));
    }
}


int processSection(const std::vector<Instruction> &instructions, int start, int end, int digit, long long z)
{
    ALU alu;
    alu.reg("z") = z;

    std::string inp{std::to_string(digit)};
    std::istringstream iss(inp);

    // Process each instruction in the section
    for (int idx = start; idx <= end; ++idx)
    {
        alu.processInstruction(instructions[idx], iss);
    }

    return alu.reg("z");
}




int main(int, char **)
{
    auto instructions = readInstructions(std::cin);

    // Split the instructions into sections, based on the 'inp' opcode.
    std::vector<std::tuple<int, int>> sections;
    int lastStart = -1;
    for (int i = 0; i < instructions.size(); ++i)
    {
        if (instructions[i].opcode == "inp")
        {
            if (lastStart >= 0)
                sections.emplace_back(std::make_tuple(lastStart, i-1));
            lastStart = i;
        }
    }
    sections.emplace_back(std::make_tuple(lastStart, instructions.size()-1));

    // Each section should be the same length. The code is repetitive with a
    // slightly different parameterization
    for (int i = 1; i < sections.size(); ++i)
        if (std::get<1>(sections[i]) - std::get<0>(sections[i]) != std::get<1>(sections[0]) - std::get<0>(sections[0]))
            throw std::runtime_error("instruction sections differ in length");

    int blockLength = std::get<1>(sections[0]) - std::get<0>(sections[0]);

    // Find instructions in regions where there is a difference
    std::set<int> differentInstructions;
    for (int i = 0; i < sections.size()-1; ++i)
    {
        for (int j = i+1; j < sections.size(); ++j)
        {
            for (int idx = 0; idx < blockLength; ++idx)
            {
                auto idx0 = idx + std::get<0>(sections[i]);
                auto idx1 = idx + std::get<0>(sections[j]);

                // Opcodes should be the same, as should the first argument
                if (instructions[idx0].opcode != instructions[idx1].opcode)
                    throw std::runtime_error("unexpected difference in opcode");
                if (instructions[idx0].x != instructions[idx1].x)
                    throw std::runtime_error("unexpected difference in first argument");

                if (instructions[idx0].y != instructions[idx1].y)
                    differentInstructions.insert(idx);
            }
        }
    }

    // Find parameters for each block
    std::vector<std::vector<int>> params;
    for (const auto &section : sections)
    {
        std::vector<int> p;
        for (const auto idx : differentInstructions)
            p.emplace_back(std::get<long long>(instructions[idx + std::get<0>(section)].y));
        params.emplace_back(p);
    }

    // The algorithm in the code simulates a stack with base 26. If the first parameter is
    // 1, then a new value is pushed onto the stack (it's actually multiplying the previous
    // value by 26, and the new value is put into the lower digit). If the first parameter
    // is 26, then we will be attempting to match the next digit and will pop that value off
    // the stack (divide by 26)

    std::string result(sections.size(), '0');
    std::vector<int> stack;
    for (int i = 0; i < sections.size(); ++i)
    {
        if (params[i][0] == 1)
        {
            // First parameter is 1 - simply add to the stack
            stack.emplace_back(i);
        }

        else if (params[i][0] == 26)
        {
            // First parameter is 26 - try to find values such that the algorithm would
            // pop the value off the stack by matching the correct digit

            // In order for the digit to match, and we need to maximize higher order
            // values, then the sum of params[i][1], params[j][2] (where j is the topmost
            // stack value, will give us two result values. We simply need to ensure that
            // both are in the range [1,9] and maximize the first
            auto j = stack.back();
            stack.pop_back();

            const auto p1 = params[i][1];
            const auto p2 = params[j][2];

            const auto Rj = std::min(9 - p1 - p2, 9);
            const auto Ri = Rj + p1 + p2;
            result[i] = char(Ri + '0');
            result[j] = char(Rj + '0');
        }
    }


    // Now that we think we have the result, run it through the ALU to confirm
    std::istringstream iss(result);

    ALU alu;
    for (const auto &section : sections)
    {
        for (int idx = std::get<0>(section); idx <= std::get<1>(section); ++idx)
        {
            alu.processInstruction(instructions[idx], iss);
        }
    }

    if (alu.reg("z") != 0)
        std::cout << "Failed to obtain model\n";
    else
        std::cout << "Result: " << result << "\n";


    return 0;
}


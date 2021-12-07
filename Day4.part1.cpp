#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>


std::vector<int> readDraw(std::istream &in)
{
    // Read the draw numbers. These are presented on the first line
    std::string line;
    in >> line;

    // Read each number, and ignore the comma (no validation on whether the comma
    // is a comma is performed)
    std::vector<int> res;
    std::istringstream iss(line);
    int num;
    while (iss >> num)
    {
        res.push_back(num);
        iss.ignore();
    }

    // Return numbers
    return res;
}


// A class to represent a bingo board
class BingoBoard
{
public:

    // Read the board from the supplied stream. Explicitly assumes 25 whitespace
    // separated numbers for a 5x5 grid
    bool readBoard(std::istream &in)
    {
        for (int i = 0; i < 25; ++i)
            if (!(in >> numbers[i/5][i%5].number))
                return false;

        return true;
    }

    // Write the board to the output stream. Add a '*' if the number has been drawn
    friend std::ostream &operator<<(std::ostream &os, const BingoBoard &board)
    {
        for (int i = 0; i < 5; i++)
        {
            for (const auto &num : board.numbers[i])
            {
                os << std::setw(4) << num.number;
                os << (num.drawn ? "*" : " ");
            }
            os << "\n";
        }
        return os;
    }

    // Play the number, and mark drawn if found
    void playNumber(int number)
    {
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if (numbers[i][j].number == number)
                    numbers[i][j].drawn = true;
    }

    // Check whether we have a winning line
    bool isComplete()
    {
        // Look for winning row
        for (int i = 0; i < 5; ++i)
            if (rowComplete(i))
                return true;

        // Look for winning columns
        for (int i = 0; i < 5; ++i)
            if (columnComplete(i))
                return true;

        return false;
    }

    // Sum the number of unplayed cells for the board score
    int sumUnplayed()
    {
        int score{0};
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j < 5; ++j)
            {
                if (!numbers[i][j].drawn)
                    score += numbers[i][j].number;
            }
        }
        return score;
    }

private:

    bool rowComplete(int row)
    {
        for (int i = 0; i < 5; ++i)
            if (!numbers[row][i].drawn)
                return false;
        return true;
    }

    bool columnComplete(int column)
    {
        for (int i = 0; i < 5; ++i)
            if (!numbers[i][column].drawn)
                return false;
        return true;
    }

    struct Number
    {
        int number;
        bool drawn{false};
    };

    Number numbers[5][5];
};




int main(int, char **)
{
    // Read the draw numbers
    auto numbers = readDraw(std::cin);

    // Read each bingo board
    std::vector<BingoBoard> boards;
    do
    {
        BingoBoard board;
        if (!board.readBoard(std::cin))
            break;
        boards.emplace_back(board);
    } while (true);

    // For all numbers drawn, play all boards, looking for the first completed board
    for (const auto &num : numbers)
    {
        std::cout << "PLAYING " << num << "\n";
        for (int i = 0; i < boards.size(); i++)
        {
            boards[i].playNumber(num);
            if (boards[i].isComplete())
            {
                std::cout << "BOARD " << i << " COMPLETE\n";
                std::cout << "SCORE: " << boards[i].sumUnplayed() * num << "\n";
                return 0;
            }
        }
    }

    return 0;
}


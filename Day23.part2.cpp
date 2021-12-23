#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>



class GameState
{
public:

    GameState() {}
    GameState(const GameState &) = default;

    void initialise(std::istream &in);

    //bool operator<(const GameState &other) const
    //{
    //    return totalEnergy_ < other.totalEnergy_;
    //}

    // Return a vector of all possible moves
    std::vector<GameState> possibleMoves() const;

    // Return the total energy for this state
    int totalEnergy() const	{ return totalEnergy_; }

    // Is this state the completed state?
    bool isComplete() const
    {
        for (int room = 0; room < 4; ++room)
        {
            const int roomPos = room*2 + 3;
            if ((board_[2][roomPos] != 'A' + room) ||
                (board_[3][roomPos] != 'A' + room) ||
                (board_[4][roomPos] != 'A' + room) ||
                (board_[5][roomPos] != 'A' + room))
                return false;
        }

        return true;
    }

private:
    // Fixed board layout in a 13x7 grid
    // #############
    // #...........#
    // ###X#X#X#X###
    //   #D#C#B#A#
    //   #D#B#A#C#
    //   #X#X#X#X#
    //   #########
    char board_[7][13];

    // Total energy used so far
    int totalEnergy_{0};

    // Move a pod into a room and return the updated GameState
    GameState moveIntoRoom(int x) const;

    // Return a vector of possible moves for an Amphipod out of a room
    std::vector<GameState> moveFromRoom(int x, int y) const;

    // The penalty for moving an amphipod of type
    static int movePenalty(char c)
    {
        return c == 'A' ? 1 : c == 'B' ? 10 : c == 'C' ? 100 : 1000;
    }

    // Is the item an amphipod
    static bool isAmphipod(char c)
    {
        return c >= 'A' && c <= 'D';
    }

};
 

void GameState::initialise(std::istream &in)
{
    // Read the input lines
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line))
        lines.push_back(line);

    // Insert the extra lines
    lines.insert(lines.begin()+3, "  #D#C#B#A#");
    lines.insert(lines.begin()+4, "  #D#B#A#C#");

    // Read the board from the input
    for (int y = 0; y < lines.size(); ++y)
    {
        const auto &line = lines[y];
        for (int x = 0; x < 13; x++)
            board_[y][x] = x < line.size() ? line[x] : ' ';
    }
}


std::vector<GameState> GameState::moveFromRoom(int x, int y) const
{
    std::vector<GameState> res;

    // Get the penalty for moving the amphipod at this location
    const auto penalty = movePenalty(board_[y][x]);

    // Moving out involves moving into the corridor, and then to the
    // various positions. First add the score
    const auto baseScore = (y-1)*penalty;

    // Move out and to the left
    for (int toX = x-1; toX > 0; --toX)
    {
        // Can we move to the new X position (i.e. not occupied). If it's
        // occupied, then we can't move any further
        if (board_[1][toX] != '.')
            break;

        // Are we at a door? If so, we can't stop here
        if (toX == 3 || toX == 5 || toX == 7 || toX == 9)
            continue;

        // Copy the current game state
        GameState newState = *this;

        // Move the amphipod in the new game state, and update the score
        // as appropriate
        newState.board_[1][toX] = newState.board_[y][x];
        newState.board_[y][x] = '.';
        newState.totalEnergy_ += baseScore + (x-toX)*penalty;
        res.emplace_back(newState);
    }

    // Move out and to the right
    for (int toX = x+1; toX <= 13; ++toX)
    {
        // Can we move to the new X position (i.e. not occupied). If it's
        // occupied, then we can't move any further
        if (board_[1][toX] != '.')
            break;

        // Are we at a door? If so, we can't stop here
        if (toX == 3 || toX == 5 || toX == 7 || toX == 9)
            continue;

        // Copy the current game state
        GameState newState = *this;

        // Move the amphipod in the new game state, and update the score
        // as appropriate
        newState.board_[1][toX] = newState.board_[y][x];
        newState.board_[y][x] = '.';
        newState.totalEnergy_ += baseScore + (toX-x)*penalty;
        res.emplace_back(newState);
    }

    return res;
}


GameState GameState::moveIntoRoom(int x) const
{
    // Get the penalty for moving the amphipod at this location
    const auto type = board_[1][x];
    const auto penalty = movePenalty(type);

    // Find the room to move into
    const auto room = (type - 'A');
    const auto roomPos = 3 + room*2;

    // Calculate the penalty to move to the room
    const int hallPenalty = abs(x - roomPos) * penalty;

    // And move into the room
    const int y = (board_[5][roomPos] == '.') ? 5 :
                  (board_[4][roomPos] == '.') ? 4 :
                  (board_[3][roomPos] == '.') ? 3 : 2;

    // Create new GameState and move the amphipod
    GameState newState = *this;

    newState.board_[y][roomPos] = board_[1][x];
    newState.board_[1][x] = '.';
    newState.totalEnergy_ += hallPenalty + (y-1)*penalty;

    return newState;
}


std::vector<GameState> GameState::possibleMoves() const
{
    // Find all possible moves and create a list of GameStates with appropriate
    // energy after each move
    std::vector<GameState> moves;

    // Firstly, setup moves out of rooms. Look for all Amphipods which are in the
    // incorrect room, or those which need to move to allow another out of the
    // room.
    for (int room = 0; room < 4; ++room)
    {
        const auto roomPos = 3 + room*2;
        const auto roomType = room + 'A';

        // Fetch room occupants
        const char occupants[] = { board_[2][roomPos], board_[3][roomPos], board_[4][roomPos], board_[5][roomPos] };

        // If they are amphipods, check that they are in the correct room. If not
        // they are candidates to move
        if ((isAmphipod(occupants[0]) && (occupants[0] != roomType)) ||
            (isAmphipod(occupants[1]) && (occupants[1] != roomType)) ||
            (isAmphipod(occupants[2]) && (occupants[2] != roomType)) ||
            (isAmphipod(occupants[3]) && (occupants[3] != roomType)))
        {
            auto newMoves = isAmphipod(occupants[0]) ? moveFromRoom(roomPos, 2) :
                            isAmphipod(occupants[1]) ? moveFromRoom(roomPos, 3) :
                            isAmphipod(occupants[2]) ? moveFromRoom(roomPos, 4) : moveFromRoom(roomPos, 5);
            moves.insert(moves.end(), newMoves.begin(), newMoves.end());
        }
    }

    // Now, look for amphipods which are in the hall and can move into a room
    for (int hallPos = 1; hallPos < 12; ++hallPos)
    {
        const auto type = board_[1][hallPos];
        if (isAmphipod(type))
        {
            const auto room = (type - 'A');
            const auto roomPos = 3 + room*2;

            // Fetch current room occupants
            const char occupants[] = { board_[2][roomPos], board_[3][roomPos], board_[4][roomPos], board_[5][roomPos]};

            // Is the room either empty, or occupied by the correct types?
            bool canEnter = true;
            for (int i = 0; i < 4; ++i)
                if (isAmphipod(occupants[i]) && occupants[i] != type)
                    canEnter = false;

            if (canEnter)
            {
               bool blocked = false;

               // Try to move into the room. Is the hallway clear?
               const auto direction = hallPos < roomPos ? 1 : -1;
               for (int x = hallPos+direction; x != roomPos && !blocked; x+=direction)
                   if (board_[1][x] != '.')
                       blocked = true;

               if (!blocked)
               {
                   auto newMove = moveIntoRoom(hallPos);
                   moves.push_back(newMove);
               }
            }
        }
    }

    return moves;
}


int bestScore(const GameState &state)
{
    if (state.isComplete())
        return state.totalEnergy();

    int best = std::numeric_limits<int>::max();

    auto possibleStates = state.possibleMoves();
    for (const auto &s : possibleStates)
    {
        if (s.totalEnergy() < best)
        {
            auto newBest = bestScore(s);
            if (newBest < best)
                best = newBest;
        }
    }

    return best;
}



int main(int, char **)
{
    // Create and initialise the game state
    GameState game;
    game.initialise(std::cin);

    std::cout << "Best score: " << bestScore(game) << "\n";

    return 0;
}


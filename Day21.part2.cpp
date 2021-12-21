#include <iostream>
#include <unordered_map>
#include <functional>


struct Player
{
    long long position{0};
    long long score{0};

    bool operator==(const Player &other) const
    {
        return position == other.position && score == other.score;
    }
};

struct State
{
    Player player1;
    Player player2;

    bool operator==(const State &other) const
    {
        return player1 == other.player1 && player2 == other.player2;
    }
};

template<>
struct std::hash<State>
{
    std::size_t operator()(const State &s) const noexcept
    {
        // We only need to consider player positions 1..10, and score up to 21
        return s.player1.position*1'000'000 + s.player1.score*10'000 +
               s.player2.position*100 + s.player2.score;
    }
};


int main(int, char **)
{
    // Simple read of input. Format is "Player X starting position: NN"
    std::string dummy;
    Player players[2];
    std::cin >> dummy >> dummy >> dummy >> dummy >> players[0].position;
    std::cin >> dummy >> dummy >> dummy >> dummy >> players[1].position;

    // Three rolls of the quantum dice will have the following results
    // Total 3: (1,1,1)
    // Total 4: (1,1,2), (1,2,1), (2,1,1)
    // Total 5: (1,1,3), (1,3,1), (3,1,1), (1,2,2), (2,1,2), (2,2,1)
    // Total 6: (1,2,3), (1,3,2), (3,1,2), (3,2,1), (3,2,1), (2,3,1), (2,2,2)
    // Total 7: (2,2,3), (2,3,2), (3,2,2), (1,3,3), (3,3,1), (3,1,3)
    // Total 8: (2,3,3), (3,2,3), (3,3,2)
    // Total 9: (3,3,3)
    std::unordered_map<long long, long long> quantumRolls = {
        {3, 1}, {4, 3}, {5, 6}, {6, 7}, {7, 6}, {8, 3}, {9, 1}
    };

    // With the qusntum dice, it's not sensible to compute all options. So we store
    // the count of each state in a map, and for each dice roll, adjust the counts
    // as necessary
    using StateMap = std::unordered_map<State, long long>;
    StateMap states;

    // Initialise with 
    State state{players[0], players[1]};
    states[state] = 1;

    // Win counts
    long long winCounts[2] = {0, 0};

    // While we still have games in progress
    int playerTurn{0};
    while (!states.empty())
    {
        // Calculate new states
        StateMap newStates;

        // For each existing state, calculate the next states following
        for (const auto &[state, universeCount] : states)
        {
            for (const auto &[rollScore, rollCount] : quantumRolls)
            {
                auto newState = state;
                Player &player = (playerTurn == 0) ? newState.player1 : newState.player2;

                player.position = ((player.position + rollScore - 1) % 10) + 1;
                player.score += player.position;

                if (player.score >= 21)
                {
                    winCounts[playerTurn] += universeCount * rollCount;
                }
                else
                {
                    newStates[newState] += universeCount * rollCount;
                }
            }
        }

        // Ready for next set of rolls
        playerTurn = 1-playerTurn;
        std::swap(states, newStates);
    }

    std::cout << "Player 1 wins: " << winCounts[0] << "\n";
    std::cout << "Player 2 wins: " << winCounts[1] << "\n";

    return 0;
}



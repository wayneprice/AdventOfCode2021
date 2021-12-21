#include <iostream>



int main(int, char **)
{
    struct Player
    {
        int position{0};
        int score{0};
    };

    // Simple read of input. Format is "Player X starting position: NN"
    std::string dummy;
    Player players[2];
    std::cin >> dummy >> dummy >> dummy >> dummy >> players[0].position;
    std::cin >> dummy >> dummy >> dummy >> dummy >> players[1].position;

    // With the deterministic dice, the sum of three rolls will be 9xN+6 where
    // N is the roll number (starting at zero). e.g. first roll will produce
    // 1+2+3 = 6 (9x0+6), second will be 4+5+6 = 15 (1*9+6), etc.
    int rollSet = 0;
    int currentPlayer = 0;

    // Play each round in turn
    while (players[0].score < 1000 && players[1].score < 1000)
    {
        Player &player = players[currentPlayer];

        int rollValue = rollSet*9 + 6;
        ++rollSet;

        player.position = ((player.position + rollValue - 1) % 10) + 1;
        player.score += player.position;
        currentPlayer = 1-currentPlayer;
    }

    std::cout << "Winning player: player " << (1-currentPlayer) + 1 << "\n";;
    std::cout << "Scores: Player1 " << players[0].score << ", Player2 " << players[1].score << "\n";
    std::cout << "Die rolled " << (rollSet*3) << "\n";

    std::cout << "Result = " << players[currentPlayer].score * rollSet * 3 << "\n";
    return 0;
}



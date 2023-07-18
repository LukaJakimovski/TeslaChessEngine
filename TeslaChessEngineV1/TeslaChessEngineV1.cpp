#include <iostream>
#include "ChessBoard.h"

int main()
{
    int dontStop;
    Chessboard board;
    board.LoadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.FindPawnMoves();
    board.FindKnightMoves();
    std::cout << "The Legal Moves in this position are: ";
    for (int i = 0; i < board.LegalMoves.size(); i++)
    {
        std::cout << board.LegalMoves[i].MoveString << ' ';
    }
    std::cin >> dontStop;
}

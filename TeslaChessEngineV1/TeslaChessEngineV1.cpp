#include <iostream>
#include "ChessBoard.h"

int main()
{
    int dontStop;
    Chessboard board;
    board.LoadPosition("8/8/2q1q3/3k4/2q1q3/8/8/8 b - - 0 1");
    board.FindPawnMoves();
    board.FindKnightMoves();
    board.FindBishopMoves();
    board.FindRookMoves();
    //board.FindQueenMoves();
    board.FindKingMoves();
    std::cout << "The Legal Moves in this position are: ";
    for (int i = 0; i < board.LegalMoves.size(); i++)
    {
        std::cout << board.LegalMoves[i].MoveString << ' ';
    }
    std::cin >> dontStop;
}

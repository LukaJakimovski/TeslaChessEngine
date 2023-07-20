#pragma once
#include "Move.h"
#include <vector>
#include <string>
#include <unordered_map>

class Chessboard {
    
public:
    Chessboard();
    char board[64];
    bool moveSide;
    char castlingAbility[4];
    int halfMoveClock;
    int fullMoveClock;
    unsigned long long enPassantTarget;
    std::vector<Move> LegalMoves; 
    unsigned long long Bitmaps[15];
    std::vector<std::vector<short>> pieceLocations;

    

    void FindLegalMoves();
    void FindPawnMoves();
    void FindKnightMoves();
    void FindBishopMoves();
    void FindRookMoves();
    void FindQueenMoves();
    void FindKingMoves();

    void LoadPosition(std::string FEN);
    void MakeMove(Move move);
    std::string BitmapsToMove(unsigned long long StartSquare, unsigned long long TargetSquare);
};

/*
short SquareToRow(std::string Square){
    return Square[1] - '0';
}

short SquareToColumn(std::string Square){
    return Square[0] - 'a' + 1;
}
*/

#pragma once
#include <vector>
#include <string>

class Chessboard {
    
public:
    Chessboard();
    char board[64];
    char moveSide;
    char castlingAbility[4];
    int halfMoveClock;
    int fullMoveClock;
    std::string enPassantTarget;
    std::string kingIndex;
    unsigned long long Bitmaps[14];
    /*
    Index 0 = WhitePawnMap
    Index 1 = WhiteKnightMap
    Index 2 = WhiteBishopMap
    Index 3 = WhiteRookMap
    Index 4 = WhiteQueenMap
    Index 5 = WhiteKingMap
    Index 6 = WhiteFullMap

    Index 7 = BlackPawnMap
    Index 8 = BlackKnightMap
    Index 9 = BlackBishopMap
    Index 10 = BlackRookMap
    Index 11 = BlackQueenMap
    Index 12 = BlackKingMap
    Index 13 = BlackFullMap
    */

    void LoadPosition(std::string FEN);
};

/*
short SquareToRow(std::string Square){
    return Square[1] - '0';
}

short SquareToColumn(std::string Square){
    return Square[0] - 'a' + 1;
}
*/

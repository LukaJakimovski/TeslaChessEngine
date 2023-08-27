#pragma once
#include "Move.h"
#include <vector>
#include <string>
#include <unordered_map>

class Chessboard {

public:
    int counter = 0;
    bool checkmate = 0;
    bool stalemate = 0;
    Chessboard();
    bool moveSide;
    char castlingAbility[4];
    int halfMoveClock;
    int fullMoveClock;
    unsigned long long enPassantTarget;
    std::vector<Move> LegalMoves;
    std::vector<Move> TrueLegalMoves;
    unsigned long long Bitmaps[15];
    unsigned long long attackedSquares[2];
    std::vector<std::string> moveStrings = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

    //Finds all pseudo legal moves
    void FindLegalMoves();
    void FindPawnMoves();
    void FindKnightMoves();
    void FindBishopMoves();
    void FindRookMoves();
    void FindQueenMoves();
    void FindKingMoves();
    void FindAllMoves();
    //Goes over Pseudo Legal Moves and returns true legal moves
    void FindTrueLegalMoves();
    void FindCastlingMoves();

    void CheckIfCheckmate();

    //Important functions for the engine.
    void LoadPosition(std::string FEN);
    void MakeMove(Move move);

    //Functions which return useful data for variables and such
    void FindAttackedSquares();
    std::string BitmapsToMove(int startIndex, int targetIndex);
    std::string BitmapsToMove(int startIndex, int targetIndex, int PromotionType);
    int BitmapToBitindex(unsigned long long Bitmap);
    int* GetCaptureType(unsigned long long targetSquare);



    //Perft Variables
    
};

/*
int SquareToRow(std::string Square){
    return Square[1] - '0';
}

int SquareToColumn(std::string Square){
    return Square[0] - 'a' + 1;
}
*/

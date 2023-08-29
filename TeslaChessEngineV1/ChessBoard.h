#pragma once
#include "Move.h"
#include <vector>
#include <string>
class Chessboard {

public:
    Chessboard();
    int* data;
    bool checkmate;
    bool stalemate;
    bool moveSide;
    char castlingAbility[4];
    int halfMoveClock;
    int fullMoveClock;

    //std::vector<Move> LegalMoves;
    Move* LegalMoves = new Move[218];
    int LegalMovesSize = 0;

    Move* TrueLegalMoves = new Move[218];
    int TrueLegalMovesSize = 0;

    unsigned long long enPassantTarget;
    unsigned long long Bitmaps[15];
    unsigned long long attackedSquares[2];


    //Functions that Find Moves
    void FindPawnMoves();
    void FindKnightMoves();
    void FindBishopMoves();
    void FindRookMoves();
    void FindQueenMoves();
    void FindKingMoves();
    void FindLegalMoves();
    void FindTrueLegalMoves();
    void FindAttackedSquares();
    void FindCastlingMoves();
    void FindAllMoves();

    void FindSlidingMoves(unsigned long long rowConstraint, unsigned long long columnConstraint, bool side, unsigned long long targetSquare, unsigned long long startSquare, int movePiece, int shiftAmount);
    //Function that updates gamestate variables
    void CheckIfCheckmate();

    //Important functions for the engine.
    void LoadPosition(std::string FEN);
    void MakeMove(Move move);

    //Functions which return useful data for variables and such
    static int BitmapToBitindex(unsigned long long Bitmap);
    void GetCaptureType(unsigned long long targetSquare);
};

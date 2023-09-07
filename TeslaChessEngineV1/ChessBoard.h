#pragma once
#include "Move.h"
#include <string>

using u64 = unsigned long long;
class Chessboard {

public:
    
    Chessboard();
    char castlingAbility[4];
    bool moveSide = 0;
    int halfMoveClock;
    int fullMoveClock;

    //Move Containers;
    Move* LegalMoves = new Move[300];
    int LegalMovesSize = 0;
    Move* TrueLegalMoves = new Move[300];
    int TrueLegalMovesSize = 0;

    //Bitmaps
    u64 enPassantTarget;
    u64 Bitmaps[15];
    u64 attackedSquares[2];

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
    void FindSlidingMoves(u64 rowConstraint, u64 columnConstraint, u64 targetSquare, int movePiece, int shiftAmount);
    
    void CreateRookBitmaps();
    void CreateBishopBitmaps();


    void FindMagicRookMoves();
    void FindMagicBishopMoves();
    void FindMagicQueenMoves();
    //Important functions for the engine.
    void LoadPosition(std::string FEN);
    void MakeMove(Move move);

    //Functions which return useful data for variables and such
    static int BitmapToBitindex(u64 Bitmap);
    static u64 BitIndexToBitmap(int bitIndex);
    void GetCaptureType(u64 targetSquare);
};

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
    Move* LegalMoves = new Move[312];
    int LegalMovesSize = 0;
    Move* TrueLegalMoves = new Move[208];
    int TrueLegalMovesSize = 0;

    int pieceCounts[15];
    int pieceLocations[65];

    //Bitmaps
    u64 enPassantTarget;
    u64 Bitmaps[15];
    u64 attackedSquares[15];

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
    
    int CheckGameState();

    void CreateRookBitmaps();
    void CreateBishopBitmaps();
    void CreateKingBitmaps();
    void CreateKnightBitmaps();
    std::string GenerateFENString();
    std::string GenerateBoardDisplay();
    int FindMoveIndexFromPGN(std::string PGN);
    //Important functions for the engine.
    void LoadPosition(std::string FEN);
    void MakeMove(Move move);

    //Functions which return useful data for variables and such
    static unsigned long BitmapToBitindex(u64 Bitmap);
    static u64 BitIndexToBitmap(int bitIndex);
    void GetCaptureType(u64 targetSquare);
    int Evaluate();
    void OrderMoves();
};

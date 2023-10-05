#include "Enums.h"
#include "Magic.h"
#include "ChessBoard.h"
#include <iostream>

using u64 = unsigned long long;
unsigned long captureIndex;
int lastSquare = 0;
u64 startSquare = 0;
u64 targetSquare = 0;

Chessboard::Chessboard() {
    moveSide = 0;
    for (int i = 0; i < 4; i++) castlingAbility[i] = 0;
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++) {
        Bitmaps[i] = 0UL;
        attackedSquares[i] = 0ULL;
        pieceCounts[i] = 0ULL;
    }
    for (int i = 0; i < 65; i++)
    {
        pieceLocations[i] = -1;
    }
}

void Chessboard::LoadPosition(std::string FEN) {
    moveSide = 0;
    for (int i = 0; i < 4; i++) castlingAbility[i] = 0;
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++) {
        Bitmaps[i] = 0UL;
        attackedSquares[i] = 0ULL;
        pieceCounts[i] = 0ULL;
    }
    for (int i = 0; i < 64; i++)
    {
        pieceLocations[i] = -1;
    }

    LegalMovesSize = 0;
    TrueLegalMovesSize = 0;
    int boardIndex = 56;
    int index = 0;
    //Set board to FEN position
    while (FEN[index] != ' ')
    {
        if (FEN[index] > '0' && FEN[index] <= '9') boardIndex += FEN[index] - '0';
        else if (FEN[index] != '/')
        {
            if (FEN[index] == 'P') {
                Bitmaps[Bitmap::WhitePawn] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhitePawn]++;
                pieceLocations[boardIndex] = Bitmap::WhitePawn;
            }
            else if (FEN[index] == 'B') {
                Bitmaps[Bitmap::WhiteBishop] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhiteBishop]++;
                pieceLocations[boardIndex] = Bitmap::WhiteBishop;
            }

            else if (FEN[index] == 'N') {
                Bitmaps[Bitmap::WhiteKnight] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhiteKnight]++;
                pieceLocations[boardIndex] = Bitmap::WhiteKnight;
            }

            else if (FEN[index] == 'R') {
                Bitmaps[Bitmap::WhiteRook] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhiteRook]++;
                pieceLocations[boardIndex] = Bitmap::WhiteRook;
            }

            else if (FEN[index] == 'Q') {
                Bitmaps[Bitmap::WhiteQueen] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhiteQueen]++;
                pieceLocations[boardIndex] = Bitmap::WhiteQueen;
            }

            else if (FEN[index] == 'K') {
                Bitmaps[Bitmap::WhiteKing] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::WhiteKing]++;
                pieceLocations[boardIndex] = Bitmap::WhiteKing;
            }

            else if (FEN[index] == 'p') {
                Bitmaps[Bitmap::BlackPawn] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackPawn]++;
                pieceLocations[boardIndex] = Bitmap::BlackPawn;
            }

            else if (FEN[index] == 'b') {
                Bitmaps[Bitmap::BlackBishop] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackBishop]++;
                pieceLocations[boardIndex] = Bitmap::BlackBishop;
            }

            else if (FEN[index] == 'n') {
                Bitmaps[Bitmap::BlackKnight] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackKnight]++;
                pieceLocations[boardIndex] = Bitmap::BlackKnight;
            }

            else if (FEN[index] == 'r') {
                Bitmaps[Bitmap::BlackRook] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackRook]++;
                pieceLocations[boardIndex] = Bitmap::BlackRook;
            }

            else if (FEN[index] == 'q') {
                Bitmaps[Bitmap::BlackQueen] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackQueen]++;
                pieceLocations[boardIndex] = Bitmap::BlackQueen;
            }

            else if (FEN[index] == 'k') {
                Bitmaps[Bitmap::BlackKing] |= (1ULL << boardIndex);
                pieceCounts[Bitmap::BlackKing]++;
                pieceLocations[boardIndex] = Bitmap::BlackKing;
            }

            boardIndex++;
        }
        if (FEN[index] == '/') {
            boardIndex -= 16;
        }
        index++;
    }
    Bitmaps[Bitmap::WhiteFull] = Bitmaps[Bitmap::WhitePawn] | Bitmaps[Bitmap::WhiteBishop] | Bitmaps[Bitmap::WhiteKnight] | Bitmaps[Bitmap::WhiteRook] | Bitmaps[Bitmap::WhiteQueen] | Bitmaps[Bitmap::WhiteKing];
    Bitmaps[Bitmap::BlackFull] = Bitmaps[Bitmap::BlackPawn] | Bitmaps[Bitmap::BlackBishop] | Bitmaps[Bitmap::BlackKnight] | Bitmaps[Bitmap::BlackRook] | Bitmaps[Bitmap::BlackQueen] | Bitmaps[Bitmap::BlackKing];
    Bitmaps[Bitmap::Full] = Bitmaps[Bitmap::WhiteFull] | Bitmaps[Bitmap::BlackFull];
    //Skip over space
    index++;
    //Set moveside to moveside
    if (FEN[index] == 'w') moveSide = 0;
    else moveSide = 1;
    //Skip to castling section
    index += 2;
    //Loop for castling ability
    while (FEN[index] != ' ')
    {
        if (FEN[index] == 'K') castlingAbility[0] = FEN[index];
        if (FEN[index] == 'Q') castlingAbility[1] = FEN[index];
        if (FEN[index] == 'k') castlingAbility[2] = FEN[index];
        if (FEN[index] == 'q') castlingAbility[3] = FEN[index];
        index++;
    }
    index++;
    //Get en-passant target square
    std::string enPassantMove = "";
    while (FEN[index] != ' ') {
        enPassantMove += FEN[index];
        index++;
    }
    index++;
    if (enPassantMove.length() == 2) enPassantTarget = 1ULL << ((enPassantMove[0] - 'a')) + (8 * (enPassantMove[1] - '0' - 1));
    //Get half move count
    int halfMoveDigits = -1;
    int tempIndex = index;
    while (FEN[tempIndex] != ' ') {
        halfMoveDigits++;
        tempIndex++;
    }
    while (FEN[index] != ' ')
    {
        halfMoveClock += (FEN[index] - '0') * (int)pow(10, halfMoveDigits);
        halfMoveDigits--;
        index++;
    }
    index++;
    //Get full move count
    int fullMoveDigits = FEN.length() - index - 1;
    while (index < FEN.length())
    {
        fullMoveClock += (FEN[index] - '0') * (int)pow(10, fullMoveDigits);
        fullMoveDigits--;
        index++;
    }
}

void Chessboard::FindLegalMoves() {
    FindPawnMoves();
    FindKnightMoves();
    FindBishopMoves();
    FindRookMoves();
    FindQueenMoves();
    FindKingMoves();
}

void Chessboard::FindAllMoves() {
    LegalMovesSize = 0;
    TrueLegalMovesSize = 0;
    FindLegalMoves();
    FindTrueLegalMoves();
    FindAttackedSquares();
    FindCastlingMoves();
}

void Chessboard::FindPawnMoves() {
    startSquare = 1UL;
    targetSquare = 1UL;
    captureIndex = 64;
    if (moveSide == 0) {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhitePawn]))
            {
                startSquare = 1ULL << i;
                //Forward
                targetSquare = startSquare << 8;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
                {
                    //Promotion
                    if (startSquare & Row::Row7) for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    //2 Squares
                    targetSquare = startSquare << 16;
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare & Row::Row2) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                }
                //Capture North West
                targetSquare = startSquare << 7;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnA) != startSquare))
                {
                    GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row7) for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }

                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureIndex = 64;
                }
                // Capture North East
                targetSquare = startSquare << 9;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column::ColumnH)) != startSquare))
                {
                    GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row7) for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }

                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureIndex = 64;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackPawn]))
            {
                startSquare = 1ULL << i;
                //Black Pawns
                //Forward
                targetSquare = startSquare >> 8;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
                {
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    targetSquare = startSquare >> 16;
                    //2 Squares
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && (startSquare & Row::Row7)) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                }
                //Capture South East
                targetSquare = startSquare >> 7;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnH) != startSquare))
                {
                    GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureIndex = 64;
                }
                // Capture South West
                targetSquare = startSquare >> 9;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnA) != startSquare))
                {
                    GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureIndex = 64;
                }
            }
        }
    }
}

void Chessboard::FindKnightMoves() {
    startSquare = 1UL;
    targetSquare = 1UL;
    captureIndex = 64;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteKnight + offSet]))
        {
            startSquare = 1ULL << i;

            //North North West
            targetSquare = startSquare << 15;
            if (((startSquare & (Row::Row7 | Row::Row8)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //North North East
            targetSquare = startSquare << 17;
            if (((startSquare & (Row::Row7 | Row::Row8)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //North West West
            targetSquare = startSquare << 6;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //North East East
            targetSquare = startSquare << 10;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South West West
            targetSquare = startSquare >> 10;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South East East
            targetSquare = startSquare >> 6;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South South West
            targetSquare = startSquare >> 17;
            if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South South East
            targetSquare = startSquare >> 15;
            if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindBishopMoves() {
    int offSet = 0;
    int squareIndex = 0;
    int magicIndex = 0;
    u64 sameSideExcluded = 0;
    unsigned long index = 0;
    if (moveSide == 1)
    {
        offSet = 7;
    }
    for (int i = 0; i < 64; i++)
    {
        if (1ULL << i & Bitmaps[Bitmap::WhiteBishop + offSet])
        {
            magicIndex = ((bishopMagics[i] * (Bitmaps[Bitmap::Full] & bishopSight[i])) >> bishopShifts[i]);
            sameSideExcluded = bishopBlockersMagics[i][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = 1ULL << i;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteBishop + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }

        }
    }
}

void Chessboard::FindRookMoves() {
    int offSet = 0;
    int squareIndex = 0;
    int magicIndex = 0;
    u64 sameSideExcluded = 0;
    unsigned long index = 0;
    if (moveSide == 1)
    {
        offSet = 7;
    }
    for (int i = 0; i < 64; i++)
    {
        if (1ULL << i & Bitmaps[Bitmap::WhiteRook + offSet])
        {
            magicIndex = ((rookMagics[i] * (Bitmaps[Bitmap::Full] & rookSight[i])) >> rookShifts[i]);
            sameSideExcluded = rookBlockersMagics[i][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = 1ULL << i;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteRook + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindQueenMoves() {
    int offSet = 0;
    int magicIndex = 0;
    u64 sameSideExcluded = 0;
    unsigned long index = 0;
    if (moveSide == 1)
    {
        offSet = 7;
    }
    for (int i = 0; i < 64; i++)
    {
        if (1ULL << i & Bitmaps[Bitmap::WhiteQueen + offSet])
        {
            magicIndex = ((bishopMagics[i] * (Bitmaps[Bitmap::Full] & bishopSight[i])) >> bishopShifts[i]);
            sameSideExcluded = bishopBlockersMagics[i][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = 1ULL << i;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteQueen + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            magicIndex = ((rookMagics[i] * (Bitmaps[Bitmap::Full] & rookSight[i])) >> rookShifts[i]);
            sameSideExcluded = rookBlockersMagics[i][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = 1ULL << i;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteQueen + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindKingMoves() {
    startSquare = 1ULL;
    targetSquare = 1ULL;
    captureIndex = 64;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteKing + offSet]))
        {
            startSquare = 1ULL << i;

            //North West
            targetSquare = startSquare << 7;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //North
            targetSquare = startSquare << 8;
            if (((startSquare & Row::Row8) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //North East
            targetSquare = startSquare << 9;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //West
            targetSquare = startSquare >> 1;
            if (((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //East
            targetSquare = startSquare << 1;
            if (((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South West
            targetSquare = startSquare >> 9;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South
            targetSquare = startSquare >> 8;
            if (((startSquare & Row::Row1) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
            //South East
            targetSquare = startSquare >> 7;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
                LegalMoves[LegalMovesSize].StartSquare = startSquare;
                LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = pieceLocations[captureIndex];
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindTrueLegalMoves() {
    u64 tempBitmaps[15] = { Bitmaps[0], Bitmaps[1], Bitmaps[2], Bitmaps[3], Bitmaps[4], Bitmaps[5], Bitmaps[6], Bitmaps[7], Bitmaps[8], Bitmaps[9], Bitmaps[10], Bitmaps[11], Bitmaps[12], Bitmaps[13], Bitmaps[14] };

    startSquare = 1ULL;
    targetSquare = 1ULL;
    int offSet = 0;
    int kingIndex = 0;
    int magicIndex = 0;
    u64 sameSideExcluded = 0;
    unsigned long index = 0;
    captureIndex = 64;

    if (moveSide == 1) offSet = 7;
    for (size_t i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::WhiteKing + offSet] & 1ULL << i) != 0)
        {
            kingIndex = i;
            break;
        }
    }

    for (size_t i = 0; i < LegalMovesSize; i++)
    {
        //For pawn promotion
        if (LegalMoves[i].PromotionType != 0)
        {
            Bitmaps[LegalMoves[i].PieceType] ^= LegalMoves[i].StartSquare;
            Bitmaps[LegalMoves[i].PromotionType] |= LegalMoves[i].TargetSquare;
        }
        //For normal moves
        else Bitmaps[LegalMoves[i].PieceType] = tempBitmaps[LegalMoves[i].PieceType] ^ LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare;
        //If a piece is captured
        if (LegalMoves[i].CaptureType > -1) Bitmaps[LegalMoves[i].CaptureType] = tempBitmaps[LegalMoves[i].CaptureType] ^ LegalMoves[i].TargetSquare;
        //Update Full Map
        Bitmaps[Bitmap::WhiteFull + offSet] = tempBitmaps[Bitmap::WhiteFull + offSet] ^ LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare;
        Bitmaps[Bitmap::Full] = tempBitmaps[Bitmap::Full] ^ LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare;
        //For EnPassant
        if ((LegalMoves[i].CaptureType == -1) && (LegalMoves[i].PieceType == Bitmap::WhitePawn || LegalMoves[i].PieceType == Bitmap::BlackPawn) && (LegalMoves[i].StartSquare << 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare << 9 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 9 == LegalMoves[i].TargetSquare))
        {
            if (LegalMoves[i].StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || LegalMoves[i].StartSquare << 1 & Bitmaps[Bitmap::WhitePawn]) {
                Bitmaps[Bitmap::WhitePawn] ^= (LegalMoves[i].TargetSquare << 8);
                Bitmaps[Bitmap::WhiteFull] ^= (LegalMoves[i].TargetSquare << 8);
                Bitmaps[Bitmap::Full] ^= (LegalMoves[i].TargetSquare << 8);
            }
            else {
                Bitmaps[Bitmap::BlackPawn] ^= (LegalMoves[i].TargetSquare >> 8);
                Bitmaps[Bitmap::BlackFull] ^= (LegalMoves[i].TargetSquare >> 8);
                Bitmaps[Bitmap::Full] ^= (LegalMoves[i].TargetSquare >> 8);
            }

        }
        //Update King Index If King Move
        if (LegalMoves[i].PieceType == Bitmap::WhiteKing + offSet)
        {
            for (int j = 0; j < 64; j++)
            {
                if ((Bitmaps[Bitmap::WhiteKing + offSet] & 1ULL << j) != 0)
                {
                    kingIndex = j;
                    break;
                }
            }
        }
        startSquare = 1ULL << kingIndex;
        //Sliding Moves
        magicIndex = ((bishopMagics[kingIndex] * (Bitmaps[Bitmap::Full] & bishopSight[kingIndex])) >> bishopShifts[kingIndex]);
        sameSideExcluded = bishopBlockersMagics[kingIndex][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
        if (LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare & bishopBlockersMagics[kingIndex][magicIndex])
        {
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                if (pieceLocations[captureIndex] == Bitmap::WhiteBishop + 7 - offSet || pieceLocations[captureIndex] == Bitmap::WhiteQueen + 7 - offSet)
                    goto False;
            }
        }
        magicIndex = ((rookMagics[kingIndex] * (Bitmaps[Bitmap::Full] & rookSight[kingIndex])) >> rookShifts[kingIndex]);
        sameSideExcluded = rookBlockersMagics[kingIndex][magicIndex] & ~Bitmaps[Bitmap::WhiteFull + offSet];
        if (LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare & bishopBlockersMagics[kingIndex][magicIndex])
        {
            while (sameSideExcluded != 0)
            {
                _BitScanReverse64(&index, sameSideExcluded);
                targetSquare = 1ULL << index;
                sameSideExcluded ^= targetSquare;
                GetCaptureType(targetSquare);
                if (pieceLocations[captureIndex] == Bitmap::WhiteRook + 7 - offSet || pieceLocations[captureIndex] == Bitmap::WhiteQueen + 7 - offSet)
                    goto False;
            }
        }
        //King
        if (kingSightPreMask[kingIndex] & Bitmaps[Bitmap::BlackKing - offSet]) {
            goto False;
        }
        //Knight
        if (knightSightPreMask[kingIndex] & Bitmaps[Bitmap::BlackKnight - offSet])
        {
            goto False;
        }
        //Pawns
        if (moveSide == 1)
        {
            //Capture South East
            if (startSquare >> 7 != 0 && ((startSquare >> 7 & (Bitmaps[Bitmap::WhitePawn])) == startSquare >> 7) && ((startSquare & Column::ColumnH) != startSquare)) goto False;
            // Capture South West
            if (startSquare >> 9 != 0 && ((startSquare >> 9 & (Bitmaps[Bitmap::WhitePawn])) == startSquare >> 9) && ((startSquare & (Column::ColumnA)) != startSquare)) goto False;
        }
        else
        {
            //Capture North West
            if (startSquare << 7 != 0 && ((startSquare << 7 & (Bitmaps[Bitmap::BlackPawn])) == startSquare << 7) && ((startSquare & Column::ColumnA) != startSquare)) goto False;
            // Capture North East
            if (startSquare << 9 != 0 && ((startSquare << 9 & (Bitmaps[Bitmap::BlackPawn])) == startSquare << 9) && ((startSquare & (Column::ColumnH)) != startSquare)) goto False;
        }
        //Move Is Legal
        TrueLegalMoves[TrueLegalMovesSize] = LegalMoves[i];
        TrueLegalMovesSize++;
    False:
        //Reset Variables
        if (LegalMoves[i].CaptureType > -1) Bitmaps[LegalMoves[i].CaptureType] = tempBitmaps[LegalMoves[i].CaptureType];
        if (LegalMoves[i].PromotionType != 0) Bitmaps[LegalMoves[i].PromotionType] = tempBitmaps[LegalMoves[i].PromotionType];
        if ((LegalMoves[i].CaptureType == -1) && (LegalMoves[i].PieceType == Bitmap::WhitePawn) && (LegalMoves[i].StartSquare << 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare << 9 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 9 == LegalMoves[i].TargetSquare)) Bitmaps[Bitmap::BlackPawn] |= LegalMoves[i].TargetSquare >> 8;
        if ((LegalMoves[i].CaptureType == -1) && (LegalMoves[i].PieceType == Bitmap::BlackPawn) && (LegalMoves[i].StartSquare << 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare << 9 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 9 == LegalMoves[i].TargetSquare)) Bitmaps[Bitmap::WhitePawn] |= LegalMoves[i].TargetSquare << 8;
    }
    for (int i = 0; i < 15; i++)
    {
        Bitmaps[i] = tempBitmaps[i];
    }
}

void Chessboard::MakeMove(Move move) {
    //This function updates Bitmaps, Castling Ability, En Passant Target, Move Side, Half/Full Move Clocks
    //For promotion
    unsigned long startIndex;
    unsigned long targetIndex;
    _BitScanForward64(&startIndex, move.StartSquare);
    _BitScanForward64(&targetIndex, move.TargetSquare);
    if (move.PromotionType != 0)
    {
        Bitmaps[move.PieceType] ^= move.StartSquare;
        Bitmaps[move.PromotionType] |= move.TargetSquare;
        pieceCounts[move.PromotionType]++;
        pieceLocations[startIndex] = -1;
        pieceLocations[targetIndex] = move.PromotionType;
    }
    //For regular move
    else {
        Bitmaps[move.PieceType] = Bitmaps[move.PieceType] ^ move.StartSquare | move.TargetSquare;
        pieceLocations[startIndex] = -1;
        pieceLocations[targetIndex] = move.PieceType;
    }
    //For E.P
    if ((move.CaptureType == -1) && (move.PieceType == Bitmap::WhitePawn || move.PieceType == Bitmap::BlackPawn) && (move.StartSquare << 7 == move.TargetSquare || move.StartSquare << 9 == move.TargetSquare || move.StartSquare >> 7 == move.TargetSquare || move.StartSquare >> 9 == move.TargetSquare))
    {
        if (move.StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || move.StartSquare << 1 & Bitmaps[Bitmap::WhitePawn]) {
            Bitmaps[Bitmap::WhitePawn] ^= (move.TargetSquare << 8);
            pieceCounts[Bitmap::WhitePawn]--;
            pieceLocations[targetIndex + 8] = -1;
        }
        else {
            Bitmaps[Bitmap::BlackPawn] ^= (move.TargetSquare >> 8);
            pieceCounts[Bitmap::BlackPawn]--;
            pieceLocations[targetIndex - 8] = -1;
        }

    }
    //For Castles
    else if (move.CaptureType == Bitmap::WhiteKing || move.CaptureType == Bitmap::BlackKing)
    {
        if (move.PieceType == Bitmap::WhiteKing)
        {
            castlingAbility[0] = '-';
            castlingAbility[1] = '-';
            if (move.TargetSquare == 1ULL << 2) {
                Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b1 | 0b1000;
                pieceLocations[0] = -1;
                pieceLocations[3] = Bitmap::WhiteRook;
            }
            else if (move.TargetSquare == 1ULL << 6) {
                Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b10000000 | 0b100000;
                pieceLocations[7] = -1;
                pieceLocations[5] = Bitmap::WhiteRook;
            }
        }
        if (move.PieceType == Bitmap::BlackKing)
        {
            castlingAbility[2] = '-';
            castlingAbility[3] = '-';
            if (move.TargetSquare == 1ULL << 62) {
                Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b1000000000000000000000000000000000000000000000000000000000000000 | 0b0010000000000000000000000000000000000000000000000000000000000000;
                pieceLocations[63] = -1;
                pieceLocations[61] = Bitmap::BlackRook;
            }
            else if (move.TargetSquare == 1ULL << 58) {
                Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b100000000000000000000000000000000000000000000000000000000 | 0b100000000000000000000000000000000000000000000000000000000000;
                pieceLocations[56] = -1;
                pieceLocations[59] = Bitmap::BlackRook;
            }
        }
    }
    //Update Captured Piece Bitmap
    else if (move.CaptureType != -1) {
        Bitmaps[move.CaptureType] ^= move.TargetSquare;
        pieceCounts[move.CaptureType]--;
    }
    // Update Castling Rights
    if (move.PieceType == Bitmap::BlackRook)
    {
        if (move.StartSquare == 1ULL << 56) castlingAbility[3] = '-';
        else if (move.StartSquare == 1ULL << 63) castlingAbility[2] = '-';
    }
    else if (move.PieceType == Bitmap::WhiteRook)
    {
        if (move.StartSquare == 1) castlingAbility[1] = '-';
        else if (move.StartSquare == 1ULL << 7) castlingAbility[0] = '-';
    }
    else if (move.PieceType == Bitmap::WhiteKing || move.PieceType == Bitmap::BlackKing)
    {
        castlingAbility[0 + moveSide + moveSide] = '-';
        castlingAbility[1 + moveSide + moveSide] = '-';
    }
    //Update EnPassant Target
    if (move.PieceType == Bitmap::WhitePawn || move.PieceType == Bitmap::BlackPawn)
    {
        if (move.TargetSquare == move.StartSquare << 16)
        {
            enPassantTarget = move.StartSquare << 8;
        }
        else if (move.TargetSquare == move.StartSquare >> 16)
        {
            enPassantTarget = move.StartSquare >> 8;
        }
        else
        {
            enPassantTarget = 0;
        }
    }
    else
    {
        enPassantTarget = 0;
    }
    //Update Move Side
    if (moveSide == 0) moveSide = 1;
    else moveSide = 0;
    //Update halfMoveClock
    if ((move.PieceType != Bitmap::WhitePawn) && (move.PieceType != Bitmap::BlackPawn) && move.CaptureType != NULL) halfMoveClock++;
    else halfMoveClock = NULL;
    //Update fullMoveClock
    if (move.PieceType >= Bitmap::BlackPawn) fullMoveClock++;
    //Reset attacked squares
    attackedSquares[Bitmap::WhiteFull] = 0;
    attackedSquares[Bitmap::BlackFull] = 0;
    //Reset Full Bitmaps
    Bitmaps[Bitmap::WhiteFull] = Bitmaps[Bitmap::WhitePawn] | Bitmaps[Bitmap::WhiteBishop] | Bitmaps[Bitmap::WhiteKnight] | Bitmaps[Bitmap::WhiteRook] | Bitmaps[Bitmap::WhiteQueen] | Bitmaps[Bitmap::WhiteKing];
    Bitmaps[Bitmap::BlackFull] = Bitmaps[Bitmap::BlackPawn] | Bitmaps[Bitmap::BlackBishop] | Bitmaps[Bitmap::BlackKnight] | Bitmaps[Bitmap::BlackRook] | Bitmaps[Bitmap::BlackQueen] | Bitmaps[Bitmap::BlackKing];
    Bitmaps[Bitmap::Full] = Bitmaps[Bitmap::WhiteFull] | Bitmaps[Bitmap::BlackFull];
}

unsigned long Chessboard::BitmapToBitindex(u64 bitMap) {
    unsigned long bitIndex = 0;
    _BitScanReverse64(&bitIndex, bitMap);
    return bitIndex;
}

u64 Chessboard::BitIndexToBitmap(int bitIndex) {
    return 1ULL << bitIndex;
}

void Chessboard::FindAttackedSquares() {
    int offSet = 0;
    if (moveSide == 0)
    {
        moveSide = 1;
        offSet = 7;
    }
    else moveSide = 0;
    int tempLegalMovesSize = LegalMovesSize;
    FindLegalMoves();
    //Check for normal pieces
    for (int i = 0; i < LegalMovesSize; i++)
    {
        if ((LegalMoves[i].PieceType < Bitmap::WhiteFull) && (LegalMoves[i].PieceType != Bitmap::WhitePawn)) {
            attackedSquares[LegalMoves[i].PieceType] |= LegalMoves[i].TargetSquare;
            attackedSquares[Bitmap::WhiteFull] |= LegalMoves[i].TargetSquare;
        }
        else if(LegalMoves[i].PieceType != Bitmap::BlackPawn) {
            attackedSquares[Bitmap::BlackFull] |= LegalMoves[i].TargetSquare;
            attackedSquares[LegalMoves[i].PieceType] |= LegalMoves[i].TargetSquare;
        }

    }
    //Check Squares Pawns Are Attacking
    for (int i = 0; i < 64; i++)
    {
        if (Bitmaps[Bitmap::WhitePawn] & (1ULL << i))
        {
            if ((Column::ColumnA & (1ULL << i)) != (1ULL << i)) {
                attackedSquares[Bitmap::WhitePawn] |= 1ULL << i << 7;
                attackedSquares[Bitmap::WhiteFull] |= 1ULL << i << 7;
            }
            if ((Column::ColumnH & (1ULL << i)) != (1ULL << i)) {
                attackedSquares[Bitmap::WhitePawn] |= 1ULL << i << 9;
                attackedSquares[Bitmap::WhiteFull] |= 1ULL << i << 9;
            }
        }
        if (Bitmaps[Bitmap::BlackPawn] & (1ULL << i))
        {
            if ((Column::ColumnH & (1ULL << i)) != (1ULL << i)) {
                attackedSquares[Bitmap::BlackPawn] |= 1ULL << i >> 7;
                attackedSquares[Bitmap::BlackFull] |= 1ULL << i >> 7;
            }
            if ((Column::ColumnA & (1ULL << i)) != (1ULL << i)) {
                attackedSquares[Bitmap::BlackPawn] |= 1ULL << i >> 9;
                attackedSquares[Bitmap::BlackFull] |= 1ULL << i >> 9;
            }
        }
    }
    LegalMovesSize = tempLegalMovesSize;
    if (moveSide == 0) moveSide = 1;
    else moveSide = 0;
}

void Chessboard::FindCastlingMoves() {
    u64 whiteKingAttacked = 0b0000000000000000000000000000000000000000000000000000000001110000;
    u64 whiteQueenAttacked = 0b0000000000000000000000000000000000000000000000000000000000011100;
    u64 blackKingAttacked = 0b0111000000000000000000000000000000000000000000000000000000000000;
    u64 blackQueenAttacked = 0b0001110000000000000000000000000000000000000000000000000000000000;

    u64 whiteKingClear = 0b000000000000000000000000000000000000000000000000000000001100000;
    u64 whiteQueenClear = 0b0000000000000000000000000000000000000000000000000000000000001110;
    u64 blackKingClear = 0b0110000000000000000000000000000000000000000000000000000000000000;
    u64 blackQueenClear = 0b0000111000000000000000000000000000000000000000000000000000000000;
    if ((castlingAbility[0] == 'K') && (whiteKingClear & Bitmaps[Bitmap::Full]) == 0 && ((whiteKingAttacked & attackedSquares[Bitmap::BlackFull]) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL << 7))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 4;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 6;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[1] == 'Q') && (whiteQueenClear & Bitmaps[Bitmap::Full]) == 0 && ((whiteQueenAttacked & attackedSquares[Bitmap::BlackFull]) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 4;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 2;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[2] == 'k') && (blackKingClear & Bitmaps[Bitmap::Full]) == 0 && ((blackKingAttacked & attackedSquares[Bitmap::WhiteFull]) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 63))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 60;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 62;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[3] == 'q') && (blackQueenClear & Bitmaps[Bitmap::Full]) == 0 && ((blackQueenAttacked & attackedSquares[Bitmap::WhiteFull]) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 56))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 60;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 58;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
}

void Chessboard::GetCaptureType(u64 targetSquare) {
    _BitScanReverse64(&captureIndex, targetSquare);
}

int Chessboard::CheckGameState() {
    //Game Not Over = 0; White Wins = 1; Black Wins = 2; Draw Via Stalemate = 3; Draw Via Repetition = 4; Draw Via 50 Move Rule = 5;
    if (TrueLegalMovesSize == 0 && moveSide == 1 && attackedSquares[Bitmap::WhiteFull] & Bitmaps[Bitmap::BlackKing])
    {
        return 1;
    }
    else if (TrueLegalMovesSize == 0 && moveSide == 0 && attackedSquares[Bitmap::BlackFull] & Bitmaps[Bitmap::WhiteKing])
    {
        return 2;
    }
    else if (TrueLegalMovesSize == 0) {
        return 3;
    }
    else if (false) {
        return 4;
    }
    else if (halfMoveClock >= 100)
    {
        return 5;
    }
    else
    {
        return 0;
    }
   
}

std::string Chessboard::GenerateBoardDisplay() {
    std::string fen = GenerateFENString();
    std::string returnString = "+---+---+---+---+---+---+---+---+\n";
    int index = 0;
    int squaresWritten = 0;
    while (squaresWritten < 64)
    {
        if (fen[index] > '0' && fen[index] < '9')
        {
            for (int i = 0; i < fen[index] - '0'; i++)
            {
                returnString.append("|   ");
                squaresWritten++;
            }
            index++;
        }
        returnString.append("|");
        returnString.append(" ");
        if (fen[index] == '/')
        {
            returnString += std::to_string(9 - squaresWritten / 8) + "\n";
            returnString += "+---+---+---+---+---+---+---+---+\n";
        }
        else
        {
            returnString.push_back(fen[index]);
            returnString.append(" ");
            squaresWritten++;
        }
        index++;
    }
    returnString.append("| 1\n+---+---+---+---+---+---+---+---+\n  a   b   c   d   e   f   g   h\n\nFEN: " + fen + "\n");
    return returnString;
}

std::string Chessboard::GenerateFENString() {
    std::unordered_map<int, std::string > PieceChars = { {Bitmap::WhitePawn, "P"}, {Bitmap::WhiteKnight, "N"}, {Bitmap::WhiteBishop, "B"}, {Bitmap::WhiteRook, "R"}, {Bitmap::WhiteQueen, "Q"},
{Bitmap::WhiteKing, "K"}, {Bitmap::BlackPawn, "p"}, {Bitmap::BlackKnight, "n"}, {Bitmap::BlackBishop, "b"}, {Bitmap::BlackRook, "r"}, {Bitmap::BlackQueen, "q"}, {Bitmap::BlackKing, "k"} };
    std::string returnString;
    int index = 56;
    int blankSpaces = 0;
    int includedValue = 0;
    std::string tempStr = "\0";
    while (true)
    {
        includedValue = 0;
        for (int i = Bitmap::WhitePawn; i <= Bitmap::WhiteKing; i++)
        {
            if (1ULL << index & Bitmaps[i])
            {
                if (blankSpaces != 0)
                {
                    returnString.append(std::to_string(blankSpaces));
                    blankSpaces = 0;
                }
                returnString.append(PieceChars[i]);
                includedValue = 1;
            }
        }
        for (int i = Bitmap::BlackPawn; i <= Bitmap::BlackKing; i++)
        {
            if (1ULL << index & Bitmaps[i])
            {
                if (blankSpaces != 0)
                {
                    returnString.append(std::to_string(blankSpaces));
                    blankSpaces = 0;
                }
                returnString.append(PieceChars[i]);
                includedValue = 1;
            }
        }
        if (includedValue == 0)
        {
            blankSpaces++;
        }
        index++;
        if (index % 8 == 0)
        {
            if (index == 8)
            {
                break;
            }
            index -= 16;
            if (blankSpaces != 0)
            {

                returnString.append(std::to_string(blankSpaces));
                blankSpaces = 0;
            }
            returnString.append("/");
        }
    }
    returnString.append(" ");
    if (moveSide == 1)
    {
        returnString.append("b");
    }
    else
    {
        returnString.append("w");
    }
    returnString.append(" ");
    for (int i = 0; i < 4; i++)
    {
        if (castlingAbility[i] != '-')
        {
            tempStr += castlingAbility[i];
        }
    }
    if (tempStr == "\0")
    {
        tempStr = "-";
    }
    returnString.append(tempStr);
    returnString.append(" ");
    tempStr = Move::GetMoveString(enPassantTarget, 0);
    if (enPassantTarget == 0)
    {
        tempStr = "-";
    }
    else
    {
        tempStr.resize(2);
    }
    returnString.append(tempStr);
    returnString.append(" ");
    returnString.append(std::to_string(halfMoveClock));
    returnString.append(" ");
    returnString.append(std::to_string(fullMoveClock));
    return returnString;
}

int Chessboard::FindMoveIndexFromPGN(std::string PGN) {
    for (int i = 0; i < TrueLegalMovesSize; i++)
    {
        if (PGN == TrueLegalMoves[i].GetLongMoveString())
        {
            return i;
        }
        
    }
    return -1;
}
#include "Enums.h"
#include "ChessBoard.h"
#include <iostream>
int captureType = -1;
int lastSquare = 0;
unsigned long long startSquare = 0;
unsigned long long targetSquare = 0;

Chessboard::Chessboard() {
    moveSide = 0;
    for (int i = 0; i < 4; i++) castlingAbility[i] = 0;
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++) Bitmaps[i] = 0UL;
    attackedSquares[0] = 0ULL;
    attackedSquares[1] = 0ULL;
}

void Chessboard::LoadPosition(std::string FEN) {
    moveSide = 0;
    for (int i = 0; i < 4; i++) castlingAbility[i] = 0;
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++) Bitmaps[i] = 0UL;
    attackedSquares[0] = 0ULL;
    attackedSquares[1] = 0ULL;

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
            if (FEN[index] == 'P') Bitmaps[Bitmap::WhitePawn] |= (1ULL << boardIndex);
            else if (FEN[index] == 'B') Bitmaps[Bitmap::WhiteBishop] |= (1ULL << boardIndex);
            else if (FEN[index] == 'N') Bitmaps[Bitmap::WhiteKnight] |= (1ULL << boardIndex);
            else if (FEN[index] == 'R') Bitmaps[Bitmap::WhiteRook] |= (1ULL << boardIndex);
            else if (FEN[index] == 'Q') Bitmaps[Bitmap::WhiteQueen] |= (1ULL << boardIndex);
            else if (FEN[index] == 'K') Bitmaps[Bitmap::WhiteKing] |= (1ULL << boardIndex);
            else if (FEN[index] == 'p') Bitmaps[Bitmap::BlackPawn] |= (1ULL << boardIndex);
            else if (FEN[index] == 'b') Bitmaps[Bitmap::BlackBishop] |= (1ULL << boardIndex);
            else if (FEN[index] == 'n') Bitmaps[Bitmap::BlackKnight] |= (1ULL << boardIndex);
            else if (FEN[index] == 'r') Bitmaps[Bitmap::BlackRook] |= (1ULL << boardIndex);
            else if (FEN[index] == 'q') Bitmaps[Bitmap::BlackQueen] |= (1ULL << boardIndex);
            else if (FEN[index] == 'k') Bitmaps[Bitmap::BlackKing] |= (1ULL << boardIndex);
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

void Chessboard::FindSlidingMoves(unsigned long long rowConstraint, unsigned long long columnConstraint, unsigned long long targetSquare, int movePiece, int shiftAmount) {
    int negative = 0b10000000000000000000000000000000;
    int offSet = 0;
    unsigned long long tempStart = 0;

    captureType = -1;
    tempStart = startSquare;
    if (moveSide == 1) offSet = 7;
    while (true)
    {
        if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
        {
            GetCaptureType(targetSquare);
            LegalMoves[LegalMovesSize].StartSquare = startSquare;
            LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
            LegalMoves[LegalMovesSize].EnPassantTarget = 0;
            LegalMoves[LegalMovesSize].PieceType = movePiece + offSet;
            LegalMoves[LegalMovesSize].CaptureType = captureType;
            LegalMoves[LegalMovesSize].PromotionType = 0;
            LegalMovesSize++;
        }
        else break;
        if (lastSquare == 1) break;
        tempStart = targetSquare;
        if ((shiftAmount & negative) == negative) targetSquare = targetSquare >> (~shiftAmount + 1);
        else targetSquare = targetSquare << shiftAmount;
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
    captureType = -1;
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
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    //2 Squares
                    targetSquare = startSquare << 16;
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare & Row::Row2) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = startSquare << 8;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                        LegalMoves[LegalMovesSize] = { startSquare, targetSquare, NULL, Bitmap::WhitePawn, captureType, j };
                        LegalMovesSize++;
                    }

                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureType = -1;
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
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }

                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::WhitePawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }

                    captureType = -1;
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
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    targetSquare = startSquare >> 16;
                    //2 Squares
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && (startSquare & Row::Row7)) {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = startSquare >> 8;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureType = -1;
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
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = j;
                        LegalMovesSize++;
                    }
                    //Normal Move
                    else {
                        LegalMoves[LegalMovesSize].StartSquare = startSquare;
                        LegalMoves[LegalMovesSize].TargetSquare = targetSquare;
                        LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                        LegalMoves[LegalMovesSize].PieceType = Bitmap::BlackPawn;
                        LegalMoves[LegalMovesSize].CaptureType = captureType;
                        LegalMoves[LegalMovesSize].PromotionType = 0;
                        LegalMovesSize++;
                    }
                    captureType = -1;
                }
            }
        }
    }
}

void Chessboard::FindKnightMoves() {
    startSquare = 1UL;
    targetSquare = 1UL;
    captureType = -1;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKnight + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindBishopMoves() {
    startSquare;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteBishop + offSet]))
        {
            startSquare = 1ULL << i;
            //North West
            FindSlidingMoves(Row::Row8, Column::ColumnA, startSquare << 7, Bitmap::WhiteBishop, 7);
            //North East
            FindSlidingMoves(Row::Row8, Column::ColumnH, startSquare << 9, Bitmap::WhiteBishop, 9);
            //South West
            FindSlidingMoves(Row::Row1, Column::ColumnA, startSquare >> 9, Bitmap::WhiteBishop, -9);
            //South East
            FindSlidingMoves(Row::Row1, Column::ColumnH, startSquare >> 7, Bitmap::WhiteBishop, -7);
        }
    }
}

void Chessboard::FindRookMoves() {
    startSquare = 1ULL;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteRook + offSet]))
        {
            startSquare = 1ULL << i;
            //North
            FindSlidingMoves(Row::Row8, NULL, startSquare << 8, Bitmap::WhiteRook, 8);
            //East
            FindSlidingMoves(NULL, Column::ColumnH, startSquare << 1, Bitmap::WhiteRook, 1);
            //South
            FindSlidingMoves(Row::Row1, NULL, startSquare >> 8, Bitmap::WhiteRook, -8);
            //West
            FindSlidingMoves(NULL, Column::ColumnA, startSquare >> 1, Bitmap::WhiteRook, -1);
        }
    }
}

void Chessboard::FindQueenMoves() {
    startSquare = 1ULL;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteQueen + offSet]))
        {
            startSquare = 1ULL << i;
            //North West
            FindSlidingMoves(Row::Row8, Column::ColumnA, startSquare << 7, Bitmap::WhiteQueen, 7);
            //North East
            FindSlidingMoves(Row::Row8, Column::ColumnH, startSquare << 9, Bitmap::WhiteQueen, 9);
            //South West
            FindSlidingMoves(Row::Row1, Column::ColumnA, startSquare >> 9, Bitmap::WhiteQueen, -9);
            //South East
            FindSlidingMoves(Row::Row1, Column::ColumnH, startSquare >> 7, Bitmap::WhiteQueen, -7);
            //North
            FindSlidingMoves(Row::Row8, NULL, startSquare << 8, Bitmap::WhiteQueen, 8);
            //East
            FindSlidingMoves(NULL, Column::ColumnH, startSquare << 1, Bitmap::WhiteQueen, 1);
            //South
            FindSlidingMoves(Row::Row1, NULL, startSquare >> 8, Bitmap::WhiteQueen, -8);
            //West
            FindSlidingMoves(NULL, Column::ColumnA, startSquare >> 1, Bitmap::WhiteQueen, -1);
        }
    }
}

void Chessboard::FindKingMoves() {
    startSquare = 1ULL;
    targetSquare = 1ULL;
    captureType = -1;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
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
                LegalMoves[LegalMovesSize].EnPassantTarget = 0;
                LegalMoves[LegalMovesSize].PieceType = Bitmap::WhiteKing + offSet;
                LegalMoves[LegalMovesSize].CaptureType = captureType;
                LegalMoves[LegalMovesSize].PromotionType = 0;
                LegalMovesSize++;
            }
        }
    }
}

void Chessboard::FindTrueLegalMoves() {
    unsigned long long tempBitmaps[15] = { Bitmaps[0], Bitmaps[1], Bitmaps[2], Bitmaps[3], Bitmaps[4], Bitmaps[5], Bitmaps[6], Bitmaps[7], Bitmaps[8], Bitmaps[9], Bitmaps[10], Bitmaps[11], Bitmaps[12], Bitmaps[13], Bitmaps[14] };

    startSquare = 1ULL;
    targetSquare = 1ULL;
    unsigned long long tempStart = 1ULL;
    int offSet = 0;
    int kingIndex = 0;
    captureType = -1;

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
        //For EnPassant
        if ((LegalMoves[i].CaptureType == -1) && (LegalMoves[i].PieceType == Bitmap::WhitePawn || LegalMoves[i].PieceType == Bitmap::BlackPawn) && (LegalMoves[i].StartSquare << 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare << 9 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 9 == LegalMoves[i].TargetSquare))
        {
            if (LegalMoves[i].StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || LegalMoves[i].StartSquare << 1 & Bitmaps[Bitmap::WhitePawn]) Bitmaps[Bitmap::WhitePawn] ^= (LegalMoves[i].TargetSquare << 8);
            else Bitmaps[Bitmap::BlackPawn] ^= (LegalMoves[i].TargetSquare >> 8);
        }
        //Update Full Map
        Bitmaps[Bitmap::WhiteFull + offSet] = tempBitmaps[Bitmap::WhiteFull + offSet] ^ LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare;
        if (LegalMoves[i].PieceType == Bitmap::WhiteKing + offSet)
        {
            for (int i = 0; i < 64; i++)
            {
                if ((Bitmaps[Bitmap::WhiteKing + offSet] & 1ULL << i) != 0)
                {
                    kingIndex = i;
                    break;
                }
            }
        }
        startSquare = 1ULL << kingIndex;
        //North West
        targetSquare = startSquare << 7;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare)) {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteBishop + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 7;
        }
        //North East
        targetSquare = startSquare << 9;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteBishop + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 9;
        }
        //South West
        targetSquare = startSquare >> 9;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteBishop + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 9;
        }
        //South East
        targetSquare = startSquare >> 7;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteBishop + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 7;
        }

        //North
        targetSquare = startSquare << 8;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteRook + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 8;
        }
        //East
        targetSquare = startSquare << 1;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteRook + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 1;
        }
        //South
        targetSquare = startSquare >> 8;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteRook + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 8;
        }
        //West
        targetSquare = startSquare >> 1;
        tempStart = startSquare;
        while (true)
        {
            if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                GetCaptureType(targetSquare);
            }
            else break;
            if (lastSquare == 1)
            {
                if (captureType == Bitmap::WhiteRook + 7 - offSet || captureType == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 1;
        }
        //North North West
        if (((startSquare & (Row::Row8 | Row::Row7)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((startSquare << 15 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare << 15)) goto False;
        //North North East
        if (((startSquare & (Row::Row8 | Row::Row7)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((startSquare << 17 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare << 17)) goto False;
        //North West West
        if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((startSquare << 6 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare << 6)) goto False;
        //North East East
        if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((startSquare << 10 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare << 10)) goto False;
        //South West West
        if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((startSquare >> 10 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare >> 10)) goto False;
        //South East East
        if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((startSquare >> 6 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare >> 6)) goto False;
        //South South West
        if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((startSquare >> 17 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare >> 17)) goto False;
        //South South East
        if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((startSquare >> 15 & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == startSquare >> 15)) goto False;
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
        TrueLegalMoves[TrueLegalMovesSize] = LegalMoves[i];
        TrueLegalMovesSize++;
    False:
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
    if (move.PromotionType != 0)
    {
        Bitmaps[move.PieceType] ^= move.StartSquare;
        Bitmaps[move.PromotionType] |= move.TargetSquare;
    }
    //For regular move
    else Bitmaps[move.PieceType] = Bitmaps[move.PieceType] ^ move.StartSquare | move.TargetSquare;
    //For E.P
    if ((move.CaptureType == -1) && (move.PieceType == Bitmap::WhitePawn || move.PieceType == Bitmap::BlackPawn) && (move.StartSquare << 7 == move.TargetSquare || move.StartSquare << 9 == move.TargetSquare || move.StartSquare >> 7 == move.TargetSquare || move.StartSquare >> 9 == move.TargetSquare))
    {
        if (move.StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || move.StartSquare << 1 & Bitmaps[Bitmap::WhitePawn]) Bitmaps[Bitmap::WhitePawn] ^= (move.TargetSquare << 8);
        else Bitmaps[Bitmap::BlackPawn] ^= (move.TargetSquare >> 8);
    }
    //For Castles
    else if (move.CaptureType == Bitmap::WhiteKing || move.CaptureType == Bitmap::BlackKing)
    {
        if (move.PieceType == Bitmap::WhiteKing)
        {
            castlingAbility[0] = '-';
            castlingAbility[1] = '-';
            if (move.TargetSquare == 1ULL << 2) Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b0000000000000000000000000000000000000000000000000000000000000001 | 0b0000000000000000000000000000000000000000000000000000000000001000;
            if (move.TargetSquare == 1ULL << 6) Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b0000000000000000000000000000000000000000000000000000000010000000 | 0b0000000000000000000000000000000000000000000000000000000000100000;
        }
        if (move.PieceType == Bitmap::BlackKing)
        {
            castlingAbility[2] = '-';
            castlingAbility[3] = '-';
            if (move.TargetSquare == 1ULL << 62) Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b1000000000000000000000000000000000000000000000000000000000000000 | 0b0010000000000000000000000000000000000000000000000000000000000000;
            if (move.TargetSquare == 1ULL << 58) Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b0000000100000000000000000000000000000000000000000000000000000000 | 0b0000100000000000000000000000000000000000000000000000000000000000;
        }
    }
    //Update Captured Piece Bitmap
    else if (move.CaptureType != -1) Bitmaps[move.CaptureType] ^= move.TargetSquare;
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
    //Update enPassant Target Square
    enPassantTarget = move.EnPassantTarget;
    //Update Move Side
    if (moveSide == 0) moveSide = 1;
    else moveSide = 0;
    //Update halfMoveClock
    if ((move.PieceType != Bitmap::WhitePawn) && (move.PieceType != Bitmap::BlackPawn) && move.CaptureType != NULL) halfMoveClock++;
    else halfMoveClock = NULL;
    //Update fullMoveClock
    if (move.PieceType >= Bitmap::BlackPawn) fullMoveClock++;
    //Reset attacked squares
    attackedSquares[0] = 0;
    attackedSquares[1] = 0;
    //Reset Full Bitmaps
    Bitmaps[Bitmap::WhiteFull] = Bitmaps[Bitmap::WhitePawn] | Bitmaps[Bitmap::WhiteBishop] | Bitmaps[Bitmap::WhiteKnight] | Bitmaps[Bitmap::WhiteRook] | Bitmaps[Bitmap::WhiteQueen] | Bitmaps[Bitmap::WhiteKing];
    Bitmaps[Bitmap::BlackFull] = Bitmaps[Bitmap::BlackPawn] | Bitmaps[Bitmap::BlackBishop] | Bitmaps[Bitmap::BlackKnight] | Bitmaps[Bitmap::BlackRook] | Bitmaps[Bitmap::BlackQueen] | Bitmaps[Bitmap::BlackKing];
    Bitmaps[Bitmap::Full] = Bitmaps[Bitmap::WhiteFull] | Bitmaps[Bitmap::BlackFull];
}

int Chessboard::BitmapToBitindex(unsigned long long bitMap) {
    int bitIndex = 0;
    while (bitMap != 1)
    {
        bitMap = bitMap >> 1;
        bitIndex++;
    }
    return bitIndex;
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
        if (LegalMoves[i].PieceType < Bitmap::WhiteFull) attackedSquares[0] |= LegalMoves[i].TargetSquare;
        else attackedSquares[1] |= LegalMoves[i].TargetSquare;
    }
    //Check Squares Pawns Are Attacking
    for (int i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::WhitePawn] & 1ULL << i))
        {
            if ((Column::ColumnA & 1ULL << i) != 1ULL << i) attackedSquares[0] |= 1ULL << i << 7;
            if ((Column::ColumnH & 1ULL << i) != 1ULL << i) attackedSquares[0] |= 1ULL << i << 9;
        }
        if ((Bitmaps[Bitmap::BlackPawn] & 1ULL << i))
        {
            if ((Column::ColumnH & 1ULL << i != 1ULL) << i) attackedSquares[0] |= 1ULL << i >> 7;
            if ((Column::ColumnA & 1ULL << i != 1ULL) << i) attackedSquares[0] |= 1ULL << i >> 9;
        }
    }
    LegalMovesSize = tempLegalMovesSize;
    if (moveSide == 0) moveSide = 1;
    else moveSide = 0;
}

void Chessboard::FindCastlingMoves() {
    unsigned long long whiteKingAttacked = 0b0000000000000000000000000000000000000000000000000000000001110000;
    unsigned long long whiteQueenAttacked = 0b0000000000000000000000000000000000000000000000000000000000011100;
    unsigned long long blackKingAttacked = 0b0111000000000000000000000000000000000000000000000000000000000000;
    unsigned long long blackQueenAttacked = 0b0001110000000000000000000000000000000000000000000000000000000000;

    unsigned long long whiteKingClear = 0b000000000000000000000000000000000000000000000000000000001100000;
    unsigned long long whiteQueenClear = 0b0000000000000000000000000000000000000000000000000000000000001110;
    unsigned long long blackKingClear = 0b0110000000000000000000000000000000000000000000000000000000000000;
    unsigned long long blackQueenClear = 0b0000111000000000000000000000000000000000000000000000000000000000;
    if ((castlingAbility[0] == 'K') && (whiteKingClear & Bitmaps[Bitmap::Full]) == 0 && ((whiteKingAttacked & attackedSquares[1]) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL << 7))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 4;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 6;
        TrueLegalMoves[TrueLegalMovesSize].EnPassantTarget = 0;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[1] == 'Q') && (whiteQueenClear & Bitmaps[Bitmap::Full]) == 0 && ((whiteQueenAttacked & attackedSquares[1]) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 4;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 2;
        TrueLegalMoves[TrueLegalMovesSize].EnPassantTarget = 0;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::WhiteKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[2] == 'k') && (blackKingClear & Bitmaps[Bitmap::Full]) == 0 && ((blackKingAttacked & attackedSquares[0]) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 63))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 60;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 62;
        TrueLegalMoves[TrueLegalMovesSize].EnPassantTarget = 0;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
    if ((castlingAbility[3] == 'q') && (blackQueenClear & Bitmaps[Bitmap::Full]) == 0 && ((blackQueenAttacked & attackedSquares[0]) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 56))) {
        TrueLegalMoves[TrueLegalMovesSize].StartSquare = 1ULL << 60;
        TrueLegalMoves[TrueLegalMovesSize].TargetSquare = 1ULL << 58;
        TrueLegalMoves[TrueLegalMovesSize].EnPassantTarget = 0;
        TrueLegalMoves[TrueLegalMovesSize].PieceType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].CaptureType = Bitmap::BlackKing;
        TrueLegalMoves[TrueLegalMovesSize].PromotionType = 0;
        TrueLegalMovesSize++;
    }
}

void Chessboard::GetCaptureType(unsigned long long targetSquare) {
    int offset = 0;
    captureType = -1;
    lastSquare = 0;
    if (moveSide == 1) offset = -7;
    for (int i = Bitmap::BlackPawn + offset; i < Bitmap::BlackFull + offset; i++)
    {
        if (targetSquare & Bitmaps[i])
        {
            captureType = i;
            lastSquare = 1;
            return;
        }
    }
    return;
}



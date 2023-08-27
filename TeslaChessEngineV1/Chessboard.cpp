#include "Enums.h"
#include "ChessBoard.h"
#include <iostream>

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

void Chessboard::FindSlidingMoves(unsigned long long rowConstraint, unsigned long long columnConstraint, bool side, unsigned long long targetSquare, unsigned long long startSquare, int movePiece, int shiftAmount) {
    int negative = 0b10000000000000000000000000000000;
    int movedIndex = BitmapToBitindex(startSquare);
    int offSet = 0;
    int currentIndex = 0;
    int* data = new int[3];
    unsigned long long tempStart = 0;
    
    *data = -1;
    tempStart = startSquare;
    currentIndex = movedIndex + shiftAmount;
    if (side == 1) offSet = 7;
    while (true)
    {
        if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
        {
            data = GetCaptureType(targetSquare);
            LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex), startSquare, targetSquare, NULL, movePiece + offSet, *data, *(data + 1), movedIndex });
        }
        else break;
        if (*(data + 2) == 1) break;
        tempStart = targetSquare;
        if ((shiftAmount & negative) == negative) targetSquare = targetSquare >> (~shiftAmount + 1);
        else targetSquare = targetSquare << shiftAmount;
        currentIndex += shiftAmount;
    }
    delete[] data;
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
    FindLegalMoves();
    FindAttackedSquares();
    FindTrueLegalMoves();
    FindCastlingMoves();
}

void Chessboard::FindPawnMoves() {
    unsigned long long startSquare = 1UL;
    unsigned long long targetSquare = 1UL;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    if (moveSide == 0) {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhitePawn]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;
                //Forward
                targetSquare = startSquare << 8;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
                {
                    //Promotion
                    if (startSquare & Row::Row7) for(int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    //2 Squares
                    targetSquare = startSquare << 16;
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare & Row::Row2) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 16), startSquare, targetSquare, startSquare << 8, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                }
                //Capture North West
                targetSquare = startSquare << 7;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnA) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row7) for(int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                // Capture North East
                targetSquare = startSquare << 9;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column::ColumnH)) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row7) for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    *data = -1;
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
                movedIndex = i;
                //Black Pawns
                //Forward
                targetSquare = startSquare >> 8;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
                {
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex});
                    targetSquare = startSquare >> 16;
                    //2 Squares
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && (startSquare & Row::Row7)) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 16), startSquare, targetSquare, startSquare >> 8, Bitmap::BlackPawn, *data, *(data + 1), movedIndex});
                }
                //Capture South East
                targetSquare = startSquare >> 7;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnH) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                // Capture South West
                targetSquare = startSquare >> 9;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column::ColumnA) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    //Promotion
                    if (startSquare & Row::Row2) for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++) LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                    //Normal Move
                    else LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::FindKnightMoves() {
    unsigned long long startSquare = 1UL;
    unsigned long long targetSquare = 1UL;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteKnight + offSet]))
        {
            startSquare = 1ULL << i;
            movedIndex = i;

            //North North West
            targetSquare = startSquare << 15;
            if (((startSquare & (Row::Row7 | Row::Row8)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 15), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //North North East
            targetSquare = startSquare << 17;
            if (((startSquare & (Row::Row7 | Row::Row8)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 17), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //North West West
            targetSquare = startSquare << 6;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 6), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //North East East
            targetSquare = startSquare << 10;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 10), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //South West West
            targetSquare = startSquare >> 10;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 10), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //South East East
            targetSquare = startSquare >> 6;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 6), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //South South West
            targetSquare = startSquare >> 17;
            if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 17), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
            //South South East
            targetSquare = startSquare >> 15;
            if (((startSquare & (Row::Row1 | Row::Row2)) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 15), startSquare, targetSquare, NULL, Bitmap::WhiteKnight + offSet, *data, *(data + 1), movedIndex });
            }
        }
    }
    delete[] data;
}

void Chessboard::FindBishopMoves() {
    unsigned long long startSquare;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteBishop + offSet]))
        {
            startSquare = 1ULL << i;
            //North West
            FindSlidingMoves(Row::Row8, Column::ColumnA, moveSide, startSquare << 7, startSquare, Bitmap::WhiteBishop, 7);
            //North East
            FindSlidingMoves(Row::Row8, Column::ColumnH, moveSide, startSquare << 9, startSquare, Bitmap::WhiteBishop, 9);
            //South West
            FindSlidingMoves(Row::Row1, Column::ColumnA, moveSide, startSquare >> 9, startSquare, Bitmap::WhiteBishop, -9);
            //South East
            FindSlidingMoves(Row::Row1, Column::ColumnH, moveSide, startSquare >> 7, startSquare, Bitmap::WhiteBishop, -7);
        }
    }
}

void Chessboard::FindRookMoves() {
    unsigned long long startSquare = 1ULL;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteRook + offSet]))
        {
            startSquare = 1ULL << i;
            //North
            FindSlidingMoves(Row::Row8, NULL, moveSide, startSquare << 8, startSquare, Bitmap::WhiteRook, 8);
            //East
            FindSlidingMoves(NULL, Column::ColumnH, moveSide, startSquare << 1, startSquare, Bitmap::WhiteRook, 1);
            //South
            FindSlidingMoves(Row::Row1, NULL, moveSide, startSquare >> 8, startSquare, Bitmap::WhiteRook, -8);
            //West
            FindSlidingMoves(NULL, Column::ColumnA, moveSide, startSquare >> 1, startSquare, Bitmap::WhiteRook, -1);
        }
    }
}

void Chessboard::FindQueenMoves() {
    unsigned long long startSquare = 1ULL;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteQueen + offSet]))
        {
            startSquare = 1ULL << i;
            //North West
            FindSlidingMoves(Row::Row8, Column::ColumnA, moveSide, startSquare << 7, startSquare, Bitmap::WhiteQueen, 7);
            //North East
            FindSlidingMoves(Row::Row8, Column::ColumnH, moveSide, startSquare << 9, startSquare, Bitmap::WhiteQueen, 9);
            //South West
            FindSlidingMoves(Row::Row1, Column::ColumnA, moveSide, startSquare >> 9, startSquare, Bitmap::WhiteQueen, -9);
            //South East
            FindSlidingMoves(Row::Row1, Column::ColumnH, moveSide, startSquare >> 7, startSquare, Bitmap::WhiteQueen, -7);
            //North
            FindSlidingMoves(Row::Row8, NULL, moveSide, startSquare << 8, startSquare, Bitmap::WhiteQueen, 8);
            //East
            FindSlidingMoves(NULL, Column::ColumnH, moveSide, startSquare << 1, startSquare, Bitmap::WhiteQueen, 1);
            //South
            FindSlidingMoves(Row::Row1, NULL, moveSide, startSquare >> 8, startSquare, Bitmap::WhiteQueen, -8);
            //West
            FindSlidingMoves(NULL, Column::ColumnA, moveSide, startSquare >> 1, startSquare, Bitmap::WhiteQueen, -1);
        }
    }
}

void Chessboard::FindKingMoves() {
    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    int captureType = -1;
    int capturedIndex = -1;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    int offSet = 0;
    if (moveSide == 1) offSet = 7;
    for (int i = 0; i < 64; i++)
    {
        if ((1ULL << i & Bitmaps[Bitmap::WhiteKing + offSet]))
        {
            startSquare = 1ULL << i;
            movedIndex = i;

            //North West
            targetSquare = startSquare << 7;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //North
            targetSquare = startSquare << 8;
            if (((startSquare & Row::Row8) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //North East
            targetSquare = startSquare << 9;
            if (((startSquare & Row::Row8) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //West
            targetSquare = startSquare >> 1;
            if (((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 1), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //East
            targetSquare = startSquare << 1;
            if (((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 1), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //South West
            targetSquare = startSquare >> 9;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //South
            targetSquare = startSquare >> 8;
            if (((startSquare & Row::Row1) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
            //South East
            targetSquare = startSquare >> 7;
            if (((startSquare & Row::Row1) == 0) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
                LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7), startSquare, targetSquare, NULL, Bitmap::WhiteKing + offSet, *data, *(data + 1), movedIndex });
            }
        }
    }
    delete[] data;
}

void Chessboard::FindTrueLegalMoves() {
    unsigned long long tempBitmaps[15] = { Bitmaps[0], Bitmaps[1], Bitmaps[2], Bitmaps[3], Bitmaps[4], Bitmaps[5], Bitmaps[6], Bitmaps[7], Bitmaps[8], Bitmaps[9], Bitmaps[10], Bitmaps[11], Bitmaps[12], Bitmaps[13], Bitmaps[14] };

    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    unsigned long long tempStart = 1ULL;
    int currentIndex = 0;
    int movedIndex = 0;
    int offSet = 0;
    int moveLegal = 1;
    int kingIndex = 0;
    int* data = new int[3];
    *data = -1;

    if (moveSide == 1) offSet = 7;
    for (size_t i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::WhiteKing + offSet] & 1ULL << i) != 0)
        {
            kingIndex = i;
            break;
        }
    }
    for (size_t i = 0; i < LegalMoves.size(); i++)
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
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare)) data = GetCaptureType(targetSquare);
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 7;
            currentIndex += 7;
        }
        //North East
        targetSquare = startSquare << 9;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 9;
            currentIndex += 9;
        }
        //South West
        targetSquare = startSquare >> 9;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 9;
            currentIndex -= 9;
        }
        //South East
        targetSquare = startSquare >> 7;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 7;
            currentIndex -= 7;
        }

        //North
        targetSquare = startSquare << 8;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row8) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 8;
            currentIndex += 8;
        }
        //East
        targetSquare = startSquare << 1;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare << 1;
            currentIndex += 1;
        }
        //South
        targetSquare = startSquare >> 8;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Row::Row1) == 0) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 8;
            currentIndex -= 8;
        }
        //West
        targetSquare = startSquare >> 1;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else break;
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet) goto False;
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 1;
            currentIndex -= 1;
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
        TrueLegalMoves.push_back(LegalMoves[i]);
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
    delete[] data;
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
    else if (move.CaptureType == Bitmap::WhiteKing || move.CaptureType == Bitmap::BlackKing )
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
        else if(move.StartSquare == 1ULL << 7) castlingAbility[0] = '-';
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
    //Clear Legal Moves
    LegalMoves.clear();
    TrueLegalMoves.clear();
    //Reset attacked squares
    attackedSquares[0] = 0;
    attackedSquares[1] = 0;
    //Reset Full Bitmaps
    Bitmaps[Bitmap::WhiteFull] = Bitmaps[Bitmap::WhitePawn] | Bitmaps[Bitmap::WhiteBishop] | Bitmaps[Bitmap::WhiteKnight] | Bitmaps[Bitmap::WhiteRook] | Bitmaps[Bitmap::WhiteQueen] | Bitmaps[Bitmap::WhiteKing];
    Bitmaps[Bitmap::BlackFull] = Bitmaps[Bitmap::BlackPawn] | Bitmaps[Bitmap::BlackBishop] | Bitmaps[Bitmap::BlackKnight] | Bitmaps[Bitmap::BlackRook] | Bitmaps[Bitmap::BlackQueen] | Bitmaps[Bitmap::BlackKing];
    Bitmaps[Bitmap::Full] = Bitmaps[Bitmap::WhiteFull] | Bitmaps[Bitmap::BlackFull];
}

void Chessboard::CheckIfCheckmate() {
    int offSet = 0;
    int sideOffSet = 1;
    int kingIndex = -1;
    if (moveSide == 1)
    {
        offSet = 7;
        sideOffSet = 0;
    }
    for (int i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::WhiteKing + offSet] & 1ULL << i) != 0)
        {
            kingIndex = i;
            break;
        }
    }
    if (TrueLegalMoves.size() == 0 && (attackedSquares[sideOffSet] & (1ULL << kingIndex))) checkmate = 1;
    else if (TrueLegalMoves.size() == 0) stalemate = 1;
}

int Chessboard::BitmapToBitindex(unsigned long long bitMap) {
    int bitIndex = 0;
    while(bitMap != 1)
    {
        bitMap = bitMap >> 1;
        bitIndex++;
    }
    return bitIndex;
}

void Chessboard::FindAttackedSquares() {
    std::vector<Move> tempLegalMoves = LegalMoves;
    int offSet = 0;
    if (moveSide == 0)
    {
        moveSide = 1;
        offSet = 7;
    }
    else moveSide = 0;
    FindLegalMoves();
    int size = LegalMoves.size();
    //Check for normal pieces
    for (int i = 0; i < size; i++)
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
    LegalMoves = tempLegalMoves;
    if (moveSide == 0) moveSide = 1;
    else moveSide = 0;
}

void Chessboard::FindCastlingMoves() {
    unsigned long long whiteKingSideNonAttackedSquares = 0b0000000000000000000000000000000000000000000000000000000001110000;
    unsigned long long whiteQueenSideNonAttackedSquares = 0b0000000000000000000000000000000000000000000000000000000000011100;
    unsigned long long blackKingSideNonAttackedSquares = 0b0111000000000000000000000000000000000000000000000000000000000000;
    unsigned long long blackQueenSideNonAttackedSquares = 0b0001110000000000000000000000000000000000000000000000000000000000;

    unsigned long long whiteKingSideClearSquares = 0b000000000000000000000000000000000000000000000000000000001100000;
    unsigned long long whiteQueenSideClearSquares = 0b0000000000000000000000000000000000000000000000000000000000001110;
    unsigned long long blackKingSideClearSquares = 0b0110000000000000000000000000000000000000000000000000000000000000;
    unsigned long long blackQueenSideClearSquares = 0b0000111000000000000000000000000000000000000000000000000000000000;
    if ((castlingAbility[0] == 'K') && (whiteKingSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((whiteKingSideNonAttackedSquares & attackedSquares[1]) == 0) && ((Bitmaps[Bitmap::Full] & (whiteKingSideNonAttackedSquares ^ Bitmaps[Bitmap::WhiteKing])) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL << 7))) TrueLegalMoves.push_back(Move{ BitmapsToMove(4, 6), 1ULL << 4, 1ULL << 6, NULL, Bitmap::WhiteKing, Bitmap::WhiteKing, -1, NULL });
    if ((castlingAbility[1] == 'Q') && (whiteQueenSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((whiteQueenSideNonAttackedSquares & attackedSquares[1]) == 0) && ((Bitmaps[Bitmap::Full] & (whiteQueenSideNonAttackedSquares ^ Bitmaps[Bitmap::WhiteKing])) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL))) TrueLegalMoves.push_back(Move{ BitmapsToMove(4, 2), 1ULL << 4, 1ULL << 2, NULL, Bitmap::WhiteKing, Bitmap::WhiteKing, -1, NULL });
    if ((castlingAbility[2] == 'k') && (blackKingSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((blackKingSideNonAttackedSquares & attackedSquares[0]) == 0) && ((Bitmaps[Bitmap::Full] & (blackKingSideNonAttackedSquares ^ Bitmaps[Bitmap::BlackKing])) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 63))) TrueLegalMoves.push_back(Move{ BitmapsToMove(60, 62), 1ULL << 60, 1ULL << 62, NULL, Bitmap::BlackKing, Bitmap::BlackKing, -1, NULL });
    if ((castlingAbility[3] == 'q') && (blackQueenSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((blackQueenSideNonAttackedSquares & attackedSquares[0]) == 0) && ((Bitmaps[Bitmap::Full] & (blackQueenSideNonAttackedSquares ^ Bitmaps[Bitmap::BlackKing])) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 56))) TrueLegalMoves.push_back(Move{ BitmapsToMove(60, 58), 1ULL << 60, 1ULL << 58, NULL, Bitmap::BlackKing, Bitmap::BlackKing, -1, NULL });
}

int* Chessboard::GetCaptureType(unsigned long long targetSquare) {
    int* ptr = new int[3];
    *ptr = -1;
    *(ptr + 2) = 0;
    int offset = 0;
    if (moveSide == 1) offset = -7;
    for (int i = Bitmap::BlackPawn + offset; i < Bitmap::BlackFull + offset; i++)
    {
        if (targetSquare & Bitmaps[i])
        {
            *ptr = i;
            *(ptr + 1) = BitmapToBitindex(targetSquare);
            *(ptr + 2) = 1;
            return ptr;
        }
    }
    return ptr;
}

std::string Chessboard::BitmapsToMove(int startIndex, int targetIndex, int PromotionType) {
    std::string move = BitmapsToMove(startIndex, targetIndex);
    if (PromotionType == Bitmap::WhiteKnight || PromotionType == Bitmap::BlackKnight) move.append("n");
    else if (PromotionType == Bitmap::WhiteBishop || PromotionType == Bitmap::BlackBishop) move.append("b");
    else if (PromotionType == Bitmap::WhiteRook || PromotionType == Bitmap::BlackRook) move.append("r");
    else if (PromotionType == Bitmap::WhiteQueen || PromotionType == Bitmap::BlackQueen) move.append("q");
    return move;
}

std::string Chessboard::BitmapsToMove(int startIndex, int targetIndex) { 
    return moveStrings[startIndex] + moveStrings[targetIndex];
}
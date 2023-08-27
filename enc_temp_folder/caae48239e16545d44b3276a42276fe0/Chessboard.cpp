#include "Enums.h"
#include "ChessBoard.h"
#include <string>
#include <iostream>
#include <bitset>
#include <chrono>

Chessboard::Chessboard() {
    moveSide = 0;
    for (int i = 0; i < 4; i++)
    {
        castlingAbility[i] = 0;
    }
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++)
    {
        Bitmaps[i] = 0UL;
    }
    attackedSquares[0] = 0ULL;
    attackedSquares[1] = 0ULL;
}

void Chessboard::LoadPosition(std::string FEN) {
    moveSide = 0;
    for (int i = 0; i < 4; i++)
    {
        castlingAbility[i] = 0;
    }
    halfMoveClock = 0;
    fullMoveClock = 0;
    enPassantTarget = 0UL;
    for (int i = 0; i < 14; i++)
    {
        Bitmaps[i] = 0UL;
    }
    attackedSquares[0] = 0ULL;
    attackedSquares[1] = 0ULL;
    int boardIndex = 56;
    size_t index = 0;

    //Set board to FEN position
    while (FEN[index] != ' ')
    {
        if (FEN[index] > '0' && FEN[index] <= '9')
        {
            boardIndex += FEN[index] - '0';
        }
        else if (FEN[index] != '/')
        {
            if (FEN[index] == 'P')
            {
                Bitmaps[Bitmap::WhitePawn] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'B')
            {
                Bitmaps[Bitmap::WhiteBishop] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'N')
            {
                Bitmaps[Bitmap::WhiteKnight] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'R')
            {
                Bitmaps[Bitmap::WhiteRook] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'Q')
            {
                Bitmaps[Bitmap::WhiteQueen] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'K')
            {
                Bitmaps[Bitmap::WhiteKing] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'p')
            {
                Bitmaps[Bitmap::BlackPawn] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'b')
            {
                Bitmaps[Bitmap::BlackBishop] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'n')
            {
                Bitmaps[Bitmap::BlackKnight] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'r')
            {
                Bitmaps[Bitmap::BlackRook] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'q')
            {
                Bitmaps[Bitmap::BlackQueen] |= (1ULL << boardIndex);
            }
            else if (FEN[index] == 'k')
            {
                Bitmaps[Bitmap::BlackKing] |= (1ULL << boardIndex);
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
    if (FEN[index] == 'w')
    {
        moveSide = 0;
    }
    else
    {
        moveSide = 1;
    }

    //Skip to castling section
    index += 2;

    //Loop for castling ability
    while (FEN[index] != ' ')
    {
        if (FEN[index] == 'K')
        {
            castlingAbility[0] = FEN[index];
        }
        if (FEN[index] == 'Q')
        {
            castlingAbility[1] = FEN[index];
        }
        if (FEN[index] == 'k')
        {
            castlingAbility[2] = FEN[index];
        }
        if (FEN[index] == 'q')
        {
            castlingAbility[3] = FEN[index];
        }
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
    if (enPassantMove.length() == 2)
    {
        enPassantTarget = 1ULL << ((enPassantMove[0] - 'a')) + (8 * (enPassantMove[1] - '0' - 1));
    }

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
    FindLegalMoves();
    FindAttackedSquares();
    FindTrueLegalMoves();
    FindCastlingMoves();
}

void Chessboard::FindPawnMoves() {
    unsigned long long Column = 0b0000000100000001000000010000000100000001000000010000000100000001;
    unsigned long long startSquare = 1UL;
    unsigned long long targetSquare = 1UL;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    *(data + 1) = 0;
    *(data + 2) = 0;

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
                    if ((startSquare >= (1ULL << Row::Row7) && startSquare < (1ULL << Row::Row8)))
                    {
                        for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    //Normal Move
                    else
                    {
                        LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    }
                    //2 Squares
                    targetSquare = startSquare << 16;
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare < 65536)
                    {
                        LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 16), startSquare, targetSquare, startSquare << 8, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    }
                }

                //Capture North West
                targetSquare = startSquare << 7;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    if ((startSquare >= (1ULL << Row::Row7) && startSquare < (1ULL << Row::Row8)))
                    {
                        for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    else
                    {
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    }
                    *data = -1;
                }
                // Capture North East
                targetSquare = startSquare << 9;
                if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column << 7)) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    if ((startSquare >= (1ULL << Row::Row7) && startSquare < (1ULL << Row::Row8)))
                    {
                        for (int j = Bitmap::WhiteKnight; j < Bitmap::WhiteKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9, j), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    else
                    {
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhitePawn, *data, *(data + 1), movedIndex });
                    }
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
                    if ((startSquare < (1ULL << Row::Row3) && startSquare >(1ULL << Row::Row1 << 7)))
                    {
                        for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    else
                    {
                        LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex});
                    }
                    targetSquare = startSquare >> 16;
                    if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare > 0b000000000000000100000000000000000000000000000000000000000000000)
                    {
                        LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 16), startSquare, targetSquare, startSquare >> 8, Bitmap::BlackPawn, *data, *(data + 1), movedIndex});
                    }
                }
                //Capture South East
                targetSquare = startSquare >> 7;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column << 7) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    if ((startSquare < (1ULL << Row::Row3) && startSquare >(1ULL << Row::Row1 << 7)))
                    {
                        for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    else
                    {
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex });
                    }
                    *data = -1;
                }
                // Capture South West
                targetSquare = startSquare >> 9;
                if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column)) != startSquare))
                {
                    data = GetCaptureType(targetSquare);
                    if ((startSquare < (1ULL << Row::Row3) && startSquare >(1ULL << Row::Row1 << 7)))
                    {
                        for (int j = Bitmap::BlackKnight; j < Bitmap::BlackKing; j++)
                        {
                            LegalMoves.emplace_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9, j), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex, j });
                        }
                    }
                    else
                    {
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9), startSquare, targetSquare, NULL, Bitmap::BlackPawn, *data, *(data + 1), movedIndex });
                    }
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
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhiteKnight]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North North West
                targetSquare = startSquare << 15;
                if ((i < Row::Row7) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 15), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North North East
                targetSquare = startSquare << 17;
                if ((i < Row::Row7) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 17), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North West West
                targetSquare = startSquare << 6;
                if ((i < Row::Row8) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 6), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North East East
                targetSquare = startSquare << 10;
                if ((i < Row::Row8) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 10), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South West West
                targetSquare = startSquare >> 10;
                if ((i >= Row::Row2) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 10), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South East East
                targetSquare = startSquare >> 6;
                if ((i >= Row::Row2) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 6), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South South West
                targetSquare = startSquare >> 17;
                if ((i >= Row::Row3) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 17), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South South East
                targetSquare = startSquare >> 15;
                if ((i >= Row::Row3) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 15), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
            }
        }
    }
    if (moveSide == 1)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackKnight]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North North West
                targetSquare = startSquare << 15;
                if ((i < Row::Row7) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 15), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North North East
                targetSquare = startSquare << 17;
                if ((i < Row::Row7) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 17), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North West West
                targetSquare = startSquare << 6;
                if ((i < Row::Row8) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 6), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North East East
                targetSquare = startSquare << 10;
                if ((i < Row::Row8) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 10), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South West West
                targetSquare = startSquare >> 10;
                if ((i >= Row::Row2) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 10), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South East East
                targetSquare = startSquare >> 6;
                if ((i >= Row::Row2) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 6), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South South West
                targetSquare = startSquare >> 17;
                if ((i >= Row::Row3) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 17), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South South East
                targetSquare = startSquare >> 15;
                if ((i >= Row::Row3) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 15), startSquare, targetSquare, NULL, Bitmap::BlackKnight, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::FindBishopMoves() {
    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    unsigned long long tempStart = 1ULL;
    int currentIndex = 0;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhiteBishop]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhiteBishop, *data, *(data + 1), movedIndex });
                        
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 7;
                    currentIndex += 7;
                }
                //North East
                targetSquare = startSquare << 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhiteBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 9;
                    currentIndex += 9;
                }
                //South West
                targetSquare = startSquare >> 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 9), startSquare, targetSquare, NULL, Bitmap::WhiteBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 9;
                    currentIndex -= 9;
                }
                //South East
                targetSquare = startSquare >> 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 7), startSquare, targetSquare, NULL, Bitmap::WhiteBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 7;
                    currentIndex -= 7;
                }
            }
        }

    }
    if (moveSide == 1)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackBishop]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 7), startSquare, targetSquare, NULL, Bitmap::BlackBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 7;
                    currentIndex += 7;
                }
                //North East
                targetSquare = startSquare << 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 9), startSquare, targetSquare, NULL, Bitmap::BlackBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 9;
                    currentIndex += 9;
                }
                //South West
                targetSquare = startSquare >> 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 9), startSquare, targetSquare, NULL, Bitmap::BlackBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 9;
                    currentIndex -= 9;
                }
                //South East
                targetSquare = startSquare >> 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 7), startSquare, targetSquare, NULL, Bitmap::BlackBishop, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 7;
                    currentIndex -= 7;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::FindRookMoves() {
    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    unsigned long long tempStart = 1ULL;
    int currentIndex = 0;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhiteRook]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North
                targetSquare = startSquare << 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhiteRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 8;
                    currentIndex += 8;
                }
                //East
                targetSquare = startSquare << 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 1), startSquare, targetSquare, NULL, Bitmap::WhiteRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 1;
                    currentIndex += 1;
                }
                //South
                targetSquare = startSquare >> 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 8), startSquare, targetSquare, NULL, Bitmap::WhiteRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 8;
                    currentIndex -= 8;
                }
                //West
                targetSquare = startSquare >> 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 1), startSquare, targetSquare, NULL, Bitmap::WhiteRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 1;
                    currentIndex -= 1;
                }
            }
        }
    }
    if (moveSide == 1)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackRook]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North
                targetSquare = startSquare << 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 8), startSquare, targetSquare, NULL, Bitmap::BlackRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 8;
                    currentIndex += 8;
                }
                //East
                targetSquare = startSquare << 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 1), startSquare, targetSquare, NULL, Bitmap::BlackRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 1;
                    currentIndex += 1;
                }
                //South
                targetSquare = startSquare >> 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 8), startSquare, targetSquare, NULL, Bitmap::BlackRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 8;
                    currentIndex -= 8;
                }
                //West
                targetSquare = startSquare >> 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 1), startSquare, targetSquare, NULL, Bitmap::BlackRook, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 1;
                    currentIndex -= 1;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::FindQueenMoves() {
    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    unsigned long long tempStart = 1ULL;
    int currentIndex = 0;
    int movedIndex = 0;
    // 0 = captureType, 1 = capturedIndex, 2 = lastSquare
    int* data = new int[3];
    *data = -1;
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhiteQueen]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 7;
                    currentIndex += 7;
                }
                //North East
                targetSquare = startSquare << 9;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 9;
                    currentIndex += 9;
                }
                //South West
                targetSquare = startSquare >> 9;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 9), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 9;
                    currentIndex -= 9;
                }
                //South East
                targetSquare = startSquare >> 7;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 7), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 7;
                    currentIndex -= 7;
                }

                //North
                targetSquare = startSquare << 8;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 8;
                    currentIndex += 8;
                }
                //East
                targetSquare = startSquare << 1;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 1), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 1;
                    currentIndex += 1;
                }
                //South
                targetSquare = startSquare >> 8;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 8), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 8;
                    currentIndex -= 8;
                }
                //West
                targetSquare = startSquare >> 1;
                tempStart = startSquare;
                currentIndex = i;
                while (true)
                {
                    if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 1), startSquare, targetSquare, NULL, Bitmap::WhiteQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 1;
                    currentIndex -= 1;
                }
            }
        }
    }
    if (moveSide == 1)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackQueen]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 7), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 7;
                    currentIndex += 7;
                }
                //North East
                targetSquare = startSquare << 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 9), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 9;
                    currentIndex += 9;
                }
                //South West
                targetSquare = startSquare >> 9;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 9), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 9;
                    currentIndex -= 9;
                }
                //South East
                targetSquare = startSquare >> 7;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 7), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 7;
                    currentIndex -= 7;
                }

                //North
                targetSquare = startSquare << 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 8), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 8;
                    currentIndex += 8;
                }
                //East
                targetSquare = startSquare << 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex + 1), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare << 1;
                    currentIndex += 1;
                }
                //South
                targetSquare = startSquare >> 8;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if ((currentIndex >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 8), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 8;
                    currentIndex -= 8;
                }
                //West
                targetSquare = startSquare >> 1;
                tempStart = startSquare;
                currentIndex = i;

                while (true)
                {
                    if (((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                    {
                        data = GetCaptureType(targetSquare);
                        LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, currentIndex - 1), startSquare, targetSquare, NULL, Bitmap::BlackQueen, *data, *(data + 1), movedIndex });
                    }
                    else
                    {
                        break;
                    }
                    if (*(data + 2) == 1)
                    {
                        *data = -1;
                        break;
                    }
                    tempStart = targetSquare;
                    targetSquare = targetSquare >> 1;
                    currentIndex -= 1;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::FindKingMoves() {
    unsigned long long startSquare = 1ULL;
    unsigned long long targetSquare = 1ULL;
    int captureType = -1;
    int capturedIndex = -1;
    int movedIndex = 0;
    int* data = new int[3];
    *data = -1;
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::WhiteKing]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                if ((i < Row::Row8) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North
                targetSquare = startSquare << 8;
                if ((i < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North East
                targetSquare = startSquare << 9;
                if ((i < Row::Row8) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //West
                targetSquare = startSquare >> 1;
                if (((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 1), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //East
                targetSquare = startSquare << 1;
                if (((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 1), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South West
                targetSquare = startSquare >> 9;
                if ((i >= Row::Row2) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South
                targetSquare = startSquare >> 8;
                if ((i >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South East
                targetSquare = startSquare >> 7;
                if ((i >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7), startSquare, targetSquare, NULL, Bitmap::WhiteKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
            }
        }
    }
    if (moveSide == 1)
    {
        for (int i = 0; i < 64; i++)
        {
            if ((1ULL << i & Bitmaps[Bitmap::BlackKing]))
            {
                startSquare = 1ULL << i;
                movedIndex = i;

                //North West
                targetSquare = startSquare << 7;
                if ((i < Row::Row8) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 7), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North
                targetSquare = startSquare << 8;
                if ((i < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 8), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //North East
                targetSquare = startSquare << 9;
                if ((i < Row::Row8) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 9), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //West
                targetSquare = startSquare >> 1;
                if (((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 1), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //East
                targetSquare = startSquare << 1;
                if (((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex + 1), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South West
                targetSquare = startSquare >> 9;
                if ((i >= Row::Row2) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 9), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South
                targetSquare = startSquare >> 8;
                if ((i >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 8), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
                //South East
                targetSquare = startSquare >> 7;
                if ((i >= Row::Row2) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
                {
                    data = GetCaptureType(targetSquare);
                    LegalMoves.push_back(Move{ BitmapsToMove(movedIndex, movedIndex - 7), startSquare, targetSquare, NULL, Bitmap::BlackKing, *data, *(data + 1), movedIndex });
                    *data = -1;
                }
            }
        }
    }
    delete[] data;
}

void Chessboard::MakeMove(Move move) {
    //This function changes Bitmaps, Castling Ability, En Passant Target, Move Side



    //For promotion
    if (move.PromotionType != 0)
    {
        Bitmaps[move.PieceType] ^= move.StartSquare;
        Bitmaps[move.PromotionType] |= move.TargetSquare;
    }
    //For regular move
    else
    {
        Bitmaps[move.PieceType] = Bitmaps[move.PieceType] ^ move.StartSquare | move.TargetSquare;
    }
    //For E.P
    if ((move.CaptureType == -1) && (move.PieceType == Bitmap::WhitePawn || move.PieceType == Bitmap::BlackPawn) && (move.StartSquare << 7 == move.TargetSquare || move.StartSquare << 9 == move.TargetSquare || move.StartSquare >> 7 == move.TargetSquare || move.StartSquare >> 9 == move.TargetSquare))
    {
        if (move.StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || move.StartSquare << 1 & Bitmaps[Bitmap::WhitePawn])
        {
            Bitmaps[Bitmap::WhitePawn] ^= (move.TargetSquare << 8);
        }
        else
        {
            Bitmaps[Bitmap::BlackPawn] ^= (move.TargetSquare >> 8);
        }
    }
    //For Castles
    else if (move.CaptureType == Bitmap::WhiteKing || move.CaptureType == Bitmap::BlackKing )
    {
        if (move.PieceType == Bitmap::WhiteKing)
        {
            castlingAbility[0] = '-';
            castlingAbility[1] = '-';
            if (move.TargetSquare == 1ULL << 2)
            {
                Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b0000000000000000000000000000000000000000000000000000000000000001 | 0b0000000000000000000000000000000000000000000000000000000000001000;
            }
            if (move.TargetSquare == 1ULL << 6)
            {
                Bitmaps[Bitmap::WhiteRook] = Bitmaps[Bitmap::WhiteRook] ^ 0b0000000000000000000000000000000000000000000000000000000010000000 | 0b0000000000000000000000000000000000000000000000000000000000100000;
            }
        }
        if (move.PieceType == Bitmap::BlackKing)
        {
            castlingAbility[2] = '-';
            castlingAbility[3] = '-';
            if (move.TargetSquare == 1ULL << 62)
            {
                Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b1000000000000000000000000000000000000000000000000000000000000000 | 0b0010000000000000000000000000000000000000000000000000000000000000;
            }
            if (move.TargetSquare == 1ULL << 58)
            {
                Bitmaps[Bitmap::BlackRook] = Bitmaps[Bitmap::BlackRook] ^ 0b0000000100000000000000000000000000000000000000000000000000000000 | 0b0000100000000000000000000000000000000000000000000000000000000000;
            }
        }
    }
    //Update Captured Piece Bitmap
    else if (move.CaptureType != -1)
    {
        Bitmaps[move.CaptureType] ^= move.TargetSquare;
    }
    // Update Castling Rights
    if (move.PieceType == Bitmap::BlackRook)
    {
        if (move.StartSquare == 1ULL << 56)
        {
            castlingAbility[3] = '-';
        }
        else if(move.StartSquare == 1ULL << 63)
        {
            castlingAbility[2] = '-';
        }
    }
    else if (move.PieceType == Bitmap::WhiteRook)
    {
        if (move.StartSquare == 1)
        {
            castlingAbility[1] = '-';
        }
        else if(move.StartSquare == 1ULL << 7)
        {
            castlingAbility[0] = '-';
        }
    }
    else if(move.PieceType == Bitmap::WhiteKing || move.PieceType == Bitmap::BlackKing)
    {
        castlingAbility[0 + moveSide + moveSide] = '-';
        castlingAbility[1 + moveSide + moveSide] = '-';
    }

    
    
    //Update enPassant Target Square
    enPassantTarget = move.EnPassantTarget;

    //Update Move Side
    if (moveSide == 0)
    {
        moveSide = 1;
    }
    else
    {
        moveSide = 0;
    }
    //Update halfMoveClock
    if ((move.PieceType != Bitmap::WhitePawn) && (move.PieceType != Bitmap::BlackPawn) && move.CaptureType != NULL)
    {
        halfMoveClock++;
    }
    else
    {
        halfMoveClock = NULL;
    }
    //Update fullMoveClock
    if (move.PieceType >= Bitmap::BlackPawn)
    {
        fullMoveClock++;
    }
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
    *(data + 1) = 0;
    *(data + 2) = 0;

    if (moveSide == 1)
    {
        offSet = 7;
    }
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
        else
        {
            Bitmaps[LegalMoves[i].PieceType] = tempBitmaps[LegalMoves[i].PieceType] ^ LegalMoves[i].StartSquare | LegalMoves[i].TargetSquare;
        }
        //If a piece is captured
        if (LegalMoves[i].CaptureType > -1)
        {
            Bitmaps[LegalMoves[i].CaptureType] = tempBitmaps[LegalMoves[i].CaptureType] ^ LegalMoves[i].TargetSquare;
            *data = -1;
        }
        //For EnPassant
        if ((LegalMoves[i].CaptureType == -1) && (LegalMoves[i].PieceType == Bitmap::WhitePawn || LegalMoves[i].PieceType == Bitmap::BlackPawn) && (LegalMoves[i].StartSquare << 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare << 9 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 7 == LegalMoves[i].TargetSquare || LegalMoves[i].StartSquare >> 9 == LegalMoves[i].TargetSquare))
        {
            if (LegalMoves[i].StartSquare >> 1 & Bitmaps[Bitmap::WhitePawn] || LegalMoves[i].StartSquare << 1 & Bitmaps[Bitmap::WhitePawn])
            {
                Bitmaps[Bitmap::WhitePawn] ^= (LegalMoves[i].TargetSquare << 8);
            }
            else
            {
                Bitmaps[Bitmap::BlackPawn] ^= (LegalMoves[i].TargetSquare >> 8);
            }
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
        movedIndex = 0;
    //North West
        targetSquare = startSquare << 7;
        tempStart = startSquare;
        currentIndex = kingIndex;
        while (true)
        {
            if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            if ((currentIndex < Row::Row8) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnA) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            if ((currentIndex >= Row::Row2) && ((tempStart & Column::ColumnH) != tempStart) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteBishop + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            if ((currentIndex < Row::Row8) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            if ((currentIndex >= Row::Row2) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull + offSet])) == targetSquare))
            {
                data = GetCaptureType(targetSquare);
            }
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
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
            else
            {
                break;
            }
            if (*(data + 2) == 1)
            {
                if (*data == Bitmap::WhiteRook + 7 - offSet || *data == Bitmap::WhiteQueen + 7 - offSet)
                {
                    moveLegal = 0;
                    goto False;
                }
                else break;
            }
            tempStart = targetSquare;
            targetSquare = targetSquare >> 1;
            currentIndex -= 1;
        }

        //North North West
        targetSquare = startSquare << 15;
        if (targetSquare != 0 && (kingIndex < Row::Row7) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //North North East
        targetSquare = startSquare << 17;
        if (targetSquare != 0 && (kingIndex < Row::Row7) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //North West West
        targetSquare = startSquare << 6;
        if (targetSquare != 0 && (kingIndex < Row::Row8) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //North East East
        targetSquare = startSquare << 10;
        if (targetSquare != 0 && (kingIndex < Row::Row8) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //South West West
        targetSquare = startSquare >> 10;
        if (targetSquare != 0 && (kingIndex >= Row::Row2) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //South East East
        targetSquare = startSquare >> 6;
        if (targetSquare != 0 && (kingIndex >= Row::Row2) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //South South West
        targetSquare = startSquare >> 17;
        if (targetSquare != 0 && (kingIndex >= Row::Row3) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //South South East
        targetSquare = startSquare >> 15;
        if (targetSquare != 0 && (kingIndex >= Row::Row3) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & (Bitmaps[Bitmap::WhiteKnight + 7 - offSet])) == targetSquare))
        {
            moveLegal = 0;
            goto False;
        }
        //Pawns
        if (moveSide == 1)
        {
            //Capture South East
            targetSquare = startSquare >> 7;
            if (targetSquare != 0 && ((targetSquare & (Bitmaps[Bitmap::WhitePawn])) == targetSquare) && ((startSquare & Column::ColumnH) != startSquare))
            {
                moveLegal = 0;
                goto False;
            }
            // Capture South West
            targetSquare = startSquare >> 9;
            if (targetSquare != 0 && ((targetSquare & (Bitmaps[Bitmap::WhitePawn])) == targetSquare) && ((startSquare & (Column::ColumnA)) != startSquare))
            {
                moveLegal = 0;
                goto False;
            }
        }
        else
        {
            //Capture North West
            targetSquare = startSquare << 7;
            if (targetSquare != 0 && ((targetSquare & (Bitmaps[Bitmap::BlackPawn])) == targetSquare) && ((startSquare & Column::ColumnA) != startSquare))
            {
                moveLegal = 0;
                goto False;
            }
            // Capture North East
            targetSquare = startSquare << 9;
            if (targetSquare != 0 && ((targetSquare & (Bitmaps[Bitmap::BlackPawn])) == targetSquare) && ((startSquare & (Column::ColumnH)) != startSquare))
            {
                moveLegal = 0;
                goto False;
            }
        }

        TrueLegalMoves.push_back(LegalMoves[i]);
    False:
        i = i;
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
    if (TrueLegalMoves.size() == 0 && (attackedSquares[sideOffSet] & (1ULL << kingIndex)))
    {
        checkmate = 1;
    }
    else if (TrueLegalMoves.size() == 0)
    {
        stalemate = 1;
    }
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
    else
    {
        moveSide = 0;
    }
    LegalMoves.clear();
    FindLegalMoves();
    int size = LegalMoves.size();
    for (int i = 0; i < size; i++)
    {
        if (LegalMoves[i].PieceType < Bitmap::WhiteFull)
        {
            attackedSquares[0] |= LegalMoves[i].TargetSquare;
        }
        else
        {
            attackedSquares[1] |= LegalMoves[i].TargetSquare;
        }

    }

    LegalMoves = tempLegalMoves;
    size = LegalMoves.size();
    for (int i = 0; i < size; i++)
    {
        if (LegalMoves[i].PieceType < Bitmap::WhiteFull)
        {
            attackedSquares[0] |= LegalMoves[i].TargetSquare;
        }
        else
        {
            attackedSquares[1] |= LegalMoves[i].TargetSquare;
        }
    }
    for (int i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::WhitePawn] & 1ULL << i))
        {
            if ((Column::ColumnA & 1ULL << i) != 1ULL << i) attackedSquares[0] |= 1ULL << i << 7;
            if ((Column::ColumnH & 1ULL << i) != 1ULL << i) attackedSquares[0] |= 1ULL << i << 9;
        }
    }
    for (int i = 0; i < 64; i++)
    {
        if ((Bitmaps[Bitmap::BlackPawn] & 1ULL << i))
        {
            if ((Column::ColumnH & 1ULL << i != 1ULL) << i) attackedSquares[0] |= 1ULL << i >> 7;
            if ((Column::ColumnA & 1ULL << i != 1ULL) << i) attackedSquares[0] |= 1ULL << i >> 9;
        }
    }
    if (moveSide == 0)
    {
        moveSide = 1;
    }
    else
    {
        moveSide = 0;
    }
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
    unsigned long long startSquare = 0;
    unsigned long long targetSquare = 0;
    if ((castlingAbility[0] == 'K') && (whiteKingSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((whiteKingSideNonAttackedSquares & attackedSquares[1]) == 0) && ((Bitmaps[Bitmap::Full] & (whiteKingSideNonAttackedSquares ^ Bitmaps[Bitmap::WhiteKing])) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL << 7)))
    {
        startSquare = 1ULL << 4;
        targetSquare = 1ULL << 6;
        TrueLegalMoves.push_back(Move{ BitmapsToMove(4, 6), startSquare, targetSquare, NULL, Bitmap::WhiteKing, Bitmap::WhiteKing, -1, NULL });
    }
    if ((castlingAbility[1] == 'Q') && (whiteQueenSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((whiteQueenSideNonAttackedSquares & attackedSquares[1]) == 0) && ((Bitmaps[Bitmap::Full] & (whiteQueenSideNonAttackedSquares ^ Bitmaps[Bitmap::WhiteKing])) == 0) && moveSide == 0 && (Bitmaps[Bitmap::WhiteRook] & (1ULL)))
    {
        startSquare = 1ULL << 4;
        targetSquare = 1ULL << 2;
        TrueLegalMoves.push_back(Move{ BitmapsToMove(4, 2), startSquare, targetSquare, NULL, Bitmap::WhiteKing, Bitmap::WhiteKing, -1, NULL });
    }
    if ((castlingAbility[2] == 'k') && (blackKingSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((blackKingSideNonAttackedSquares & attackedSquares[0]) == 0) && ((Bitmaps[Bitmap::Full] & (blackKingSideNonAttackedSquares ^ Bitmaps[Bitmap::BlackKing])) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 63)))
    {
        startSquare = 1ULL << 60;
        targetSquare = 1ULL << 62;
        TrueLegalMoves.push_back(Move{ BitmapsToMove(60, 62), startSquare, targetSquare, NULL, Bitmap::BlackKing, Bitmap::BlackKing, -1, NULL });
    }
    if ((castlingAbility[3] == 'q') && (blackQueenSideClearSquares & Bitmaps[Bitmap::Full]) == 0 && ((blackQueenSideNonAttackedSquares & attackedSquares[0]) == 0) && ((Bitmaps[Bitmap::Full] & (blackQueenSideNonAttackedSquares ^ Bitmaps[Bitmap::BlackKing])) == 0) && moveSide == 1 && (Bitmaps[Bitmap::BlackRook] & (1ULL << 56)))
    {
        startSquare = 1ULL << 60;
        targetSquare = 1ULL << 58;
        TrueLegalMoves.push_back(Move{ BitmapsToMove(60, 58), startSquare, targetSquare, NULL, Bitmap::BlackKing, Bitmap::BlackKing, -1, NULL });
    }
}

int* Chessboard::GetCaptureType(unsigned long long targetSquare) {
    int* ptr = new int[3];
    *ptr = -1;
    *(ptr + 1) = 0;
    *(ptr + 2) = 0;
    int offset = 0;
    if (moveSide == 1)
    {
        offset = -7;
    }
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
    if (PromotionType == Bitmap::WhiteKnight || PromotionType == Bitmap::BlackKnight)
    {
        move.append("n");
    }
    else if (PromotionType == Bitmap::WhiteBishop || PromotionType == Bitmap::BlackBishop)
    {
        move.append("b");
    }
    else if (PromotionType == Bitmap::WhiteRook || PromotionType == Bitmap::BlackRook)
    {
        move.append("r");
    }
    else if (PromotionType == Bitmap::WhiteQueen || PromotionType == Bitmap::BlackQueen)
    {
        move.append("q");
    }
    return move;
}

std::string Chessboard::BitmapsToMove(int startIndex, int targetIndex) { 
    return moveStrings[startIndex] + moveStrings[targetIndex];
}
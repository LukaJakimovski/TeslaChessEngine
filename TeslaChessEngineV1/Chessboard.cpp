#include "Enums.h"
#include "ChessBoard.h"

#include <string>
#include <iostream>
#include <bitset>



Chessboard::Chessboard() {;
    for (int i = 0; i < 64; i++)
    {
        board[i] = 0;
    }
    moveSide = '-';
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
    pieceLocations.resize(15);
}

void Chessboard::LoadPosition(std::string FEN) {
    short boardIndex = 56;
    short index = 0;

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
                pieceLocations[Bitmap::WhitePawn].push_back(boardIndex);
            }
            else if (FEN[index] == 'B')
            {
                Bitmaps[Bitmap::WhiteBishop] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::WhiteBishop].push_back(boardIndex);
            }
            else if (FEN[index] == 'N')
            {
                Bitmaps[Bitmap::WhiteKnight] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::WhiteKnight].push_back(boardIndex);
            }
            else if (FEN[index] == 'R')
            {
                Bitmaps[Bitmap::WhiteRook] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::WhiteRook].push_back(boardIndex);
            }
            else if (FEN[index] == 'Q')
            {
                Bitmaps[Bitmap::WhiteQueen] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::WhiteQueen].push_back(boardIndex);
            }
            else if (FEN[index] == 'K')
            {
                Bitmaps[Bitmap::WhiteKing] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::WhiteKing].push_back(boardIndex);
            }
            else if (FEN[index] == 'p')
            {
                Bitmaps[Bitmap::BlackPawn] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackPawn].push_back(boardIndex);
            }
            else if (FEN[index] == 'b')
            {
                Bitmaps[Bitmap::BlackBishop] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackBishop].push_back(boardIndex);
            }
            else if (FEN[index] == 'n')
            {
                Bitmaps[Bitmap::BlackKnight] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackKnight].push_back(boardIndex);
            }
            else if (FEN[index] == 'r')
            {
                Bitmaps[Bitmap::BlackRook] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackRook].push_back(boardIndex);
            }
            else if (FEN[index] == 'q')
            {
                Bitmaps[Bitmap::BlackQueen] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackQueen].push_back(boardIndex);
            }
            else if (FEN[index] == 'k')
            {
                Bitmaps[Bitmap::BlackKing] |= (1ULL << boardIndex);
                pieceLocations[Bitmap::BlackKing].push_back(boardIndex);
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
    /*
    std::cout << "White Pawn: " << std::bitset<64>(Bitmaps[Bitmap::WhitePawn]) << std::endl;
    std::cout << "White Bishop: " << std::bitset<64>(Bitmaps[Bitmap::WhiteBishop]) << std::endl;
    std::cout << "White Knight: " << std::bitset<64>(Bitmaps[Bitmap::WhiteKnight]) << std::endl;
    std::cout << "White Rook: " << std::bitset<64>(Bitmaps[Bitmap::WhiteRook]) << std::endl;
    std::cout << "White Queen: " << std::bitset<64>(Bitmaps[Bitmap::WhiteQueen]) << std::endl;
    std::cout << "White King: " << std::bitset<64>(Bitmaps[Bitmap::WhiteKing]) << std::endl;
    std::cout << "Full White: " << std::bitset<64>(Bitmaps[Bitmap::WhiteFull]) << std::endl;
    std::cout << "Black Pawn: " << std::bitset<64>(Bitmaps[Bitmap::BlackPawn]) << std::endl;
    std::cout << "Black Bishop: " << std::bitset<64>(Bitmaps[Bitmap::BlackBishop]) << std::endl;
    std::cout << "Black Knight: " << std::bitset<64>(Bitmaps[Bitmap::BlackKnight]) << std::endl;
    std::cout << "Black Rook: " << std::bitset<64>(Bitmaps[Bitmap::BlackRook]) << std::endl;
    std::cout << "Black Queen: " << std::bitset<64>(Bitmaps[Bitmap::BlackQueen]) << std::endl;
    std::cout << "Black King: " << std::bitset<64>(Bitmaps[Bitmap::BlackKing]) << std::endl;
    std::cout << "Full Black: " << std::bitset<64>(Bitmaps[Bitmap::BlackFull]) << std::endl;
    std::cout << "Full: " << std::bitset<64>(Bitmaps[Bitmap::Full]) << std::endl;
    */
    
    
    
    

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
        enPassantTarget = 1ULL << (enPassantMove[0] - 'a') + (8 * (enPassantMove[1] - '0' - 1));
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
        halfMoveClock += (FEN[index] - '0') * pow(10, halfMoveDigits);
        halfMoveDigits--;
        index++;
    }
    index++;
    //Get full move count
    int fullMoveDigits = FEN.length() - index - 1;
    while (index < FEN.length())
    {
        fullMoveClock += (FEN[index] - '0') * pow(10, fullMoveDigits);
        fullMoveDigits--;
        index++;
    }
}

void Chessboard::FindLegalMoves() {

}

void Chessboard::FindPawnMoves() {
    unsigned long long Column = 0b0000000100000001000000010000000100000001000000010000000100000001;
    unsigned long long startSquare = 1UL;
    unsigned long long targetSquare = 1UL;
    short captureType = -1;
    short capturedIndex = -1;
    short movedIndex = 0;

    if (moveSide == 0) {
        for (int i = 0; i < pieceLocations[Bitmap::WhitePawn].size(); i++)
        {
            startSquare = 1ULL << pieceLocations[Bitmap::WhitePawn][i];
            movedIndex = i;

            //Forward
            targetSquare = startSquare << 8;
            if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
            {
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhitePawn, captureType, capturedIndex, movedIndex });

                targetSquare = startSquare << 16;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare < 65536)
                {
                    LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, startSquare << 8, Bitmap::WhitePawn, captureType, capturedIndex, movedIndex });
                }
            }

            //Capture Left
            targetSquare = startSquare << 7;
            if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column) != startSquare))
            {
                for (int i = Bitmap::BlackPawn; i < Bitmap::BlackFull; i++)
                {
                    if (targetSquare & Bitmaps[i])
                    {
                        captureType = i;
                        for (int j = 0; j < pieceLocations[captureType].size(); j++)
                        {
                            if (pieceLocations[captureType][j])
                            {
                                capturedIndex = j;
                            }
                        }
                        break;
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhitePawn, captureType, capturedIndex, movedIndex });
                //std::cout << BitmapsToMove(startSquare, targetSquare) << " " << startSquare << " " << targetSquare << " " << Bitmap::WhitePawn << " " << captureType;
            }
            // Capture Right
            targetSquare = startSquare << 9;
            if (((targetSquare & (Bitmaps[Bitmap::BlackFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column << 7)) != startSquare))
            {
                for (int i = Bitmap::BlackPawn; i < Bitmap::BlackFull; i++)
                {
                    if ((targetSquare & Bitmaps[i]) == targetSquare)
                    {
                        captureType = i;
                        for (int j = 0; j < pieceLocations[captureType].size(); j++)
                        {
                            if (pieceLocations[captureType][j])
                            {
                                capturedIndex = j;
                            }
                        }
                        break;
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhitePawn, captureType, capturedIndex, movedIndex });
            }
        }
    }
    else
    {
        for (int i = 0; i < pieceLocations[Bitmap::BlackPawn].size(); i++)
        {
            startSquare = 1ULL << pieceLocations[Bitmap::BlackPawn][i];
            //Black Pawns
            //Forward
            targetSquare = startSquare >> 8;
            if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare))
            {
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackPawn, NULL, capturedIndex, movedIndex });

                targetSquare = startSquare >> 16;
                if (((targetSquare & ~(Bitmaps[Bitmap::Full])) == targetSquare) && startSquare > 0b000000000000000010000000000000000000000000000000000000000000000)
                {
                    LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, startSquare >> 8, Bitmap::BlackPawn, NULL, capturedIndex, movedIndex });
                }
            }
            //Capture Left
            targetSquare = startSquare >> 7;
            if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & Column << 7) != startSquare))
            {
                for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteFull; i++)
                {
                    if (targetSquare & Bitmaps[i])
                    {
                        captureType = i;
                        for (int j = 0; j < pieceLocations[captureType].size(); j++)
                        {
                            if (pieceLocations[captureType][j])
                            {
                                capturedIndex = j;
                            }
                        }
                        break;
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackPawn, captureType, capturedIndex, movedIndex });
            }
            // Capture Right
            targetSquare = startSquare >> 9;
            if (((targetSquare & (Bitmaps[Bitmap::WhiteFull] | enPassantTarget)) == targetSquare) && ((startSquare & (Column)) != startSquare))
            {
                for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteFull; i++)
                {
                    if ((targetSquare & Bitmaps[i]) == targetSquare)
                    {
                        captureType = i;
                        for (int j = 0; j < pieceLocations[captureType].size(); j++)
                        {
                            if (pieceLocations[captureType][j])
                            {
                                capturedIndex = j;
                            }
                        }
                        break;
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackPawn, captureType, capturedIndex, movedIndex });
            }
        }
    }
    
}
void Chessboard::FindKnightMoves() {
    //Row 1 on board shift left 8 to go over 1 row ex. row << 4 = row 5
    unsigned long long row = 0b0000000000000000000000000000000000000000000000000000000011111111;
    //Column a on board shift left 1 to go up 1 column ex. column << 4 = column e
    unsigned long long column = 0b0000000100000001000000010000000100000001000000010000000100000001;
    unsigned long long startSquare = 1UL;
    unsigned long long targetSquare = 1UL;
    short captureType = -1;
    short capturedIndex = -1;
    short movedIndex = 0;
    if (moveSide == 0)
    {
        for (int i = 0; i < pieceLocations[Bitmap::WhiteKnight].size(); i++)
        {
            startSquare = 1ULL << pieceLocations[Bitmap::WhiteKnight][i];

            //North North West
            targetSquare = startSquare << 15;
            if ((pieceLocations[Bitmap::WhiteKnight][i] < Row::Row7) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //North North East
            targetSquare = startSquare << 17;
            if ((pieceLocations[Bitmap::WhiteKnight][i] < Row::Row7) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //North West West
            targetSquare = startSquare << 6;
            if ((pieceLocations[Bitmap::WhiteKnight][i] < Row::Row8) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //North East East
            targetSquare = startSquare << 10;
            if ((pieceLocations[Bitmap::WhiteKnight][i] < Row::Row8) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //South West West
            targetSquare = startSquare >> 10;
            if ((pieceLocations[Bitmap::WhiteKnight][i] >= Row::Row2) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //South East East
            targetSquare = startSquare >> 6;
            if ((pieceLocations[Bitmap::WhiteKnight][i] >= Row::Row2) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //South South West
            targetSquare = startSquare >> 17;
            if ((pieceLocations[Bitmap::WhiteKnight][i] >= Row::Row3) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
            //South South East
            targetSquare = startSquare >> 15;
            if ((pieceLocations[Bitmap::WhiteKnight][i] >= Row::Row3) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::WhiteFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::BlackFull])) == targetSquare)
                {
                    for (int i = Bitmap::BlackPawn; i < Bitmap::BlackKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhiteKnight, captureType, capturedIndex, movedIndex });
            }
        }
    }
    if (moveSide == 1)
    {
        for (int i = 0; i < pieceLocations[Bitmap::BlackKnight].size(); i++)
        {
            startSquare = 1ULL << pieceLocations[Bitmap::BlackKnight][i];

            //North North West
            targetSquare = startSquare << 15;
            if ((pieceLocations[Bitmap::BlackKnight][i] < Row::Row7) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //North North East
            targetSquare = startSquare << 17;
            if ((pieceLocations[Bitmap::BlackKnight][i] < Row::Row7) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //North West West
            targetSquare = startSquare << 6;
            if ((pieceLocations[Bitmap::BlackKnight][i] < Row::Row8) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //North East East
            targetSquare = startSquare << 10;
            if ((pieceLocations[Bitmap::BlackKnight][i] < Row::Row8) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //South West West
            targetSquare = startSquare >> 10;
            if ((pieceLocations[Bitmap::BlackKnight][i] >= Row::Row2) && ((startSquare & (Column::ColumnA | Column::ColumnB)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //South East East
            targetSquare = startSquare >> 6;
            if ((pieceLocations[Bitmap::BlackKnight][i] >= Row::Row2) && ((startSquare & (Column::ColumnG | Column::ColumnH)) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
            //South South West
            targetSquare = startSquare >> 17;
            if ((pieceLocations[Bitmap::BlackKnight][i] >= Row::Row3) && ((startSquare & Column::ColumnA) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::WhitePawn, captureType, capturedIndex, movedIndex });
            }
            //South South East
            targetSquare = startSquare >> 15;
            if ((pieceLocations[Bitmap::BlackKnight][i] >= Row::Row3) && ((startSquare & Column::ColumnH) != startSquare) && ((targetSquare & ~(Bitmaps[Bitmap::BlackFull])) == targetSquare))
            {
                if ((targetSquare & (Bitmaps[Bitmap::WhiteFull])) == targetSquare)
                {
                    for (int i = Bitmap::WhitePawn; i < Bitmap::WhiteKing; i++)
                    {
                        if (targetSquare & Bitmaps[i])
                        {
                            captureType = i;
                            for (int j = 0; j < pieceLocations[captureType].size(); j++)
                            {
                                if (pieceLocations[captureType][j])
                                {
                                    capturedIndex = j;
                                }
                            }
                            break;
                        }
                    }
                }
                LegalMoves.push_back(Move{ BitmapsToMove(startSquare, targetSquare), startSquare, targetSquare, NULL, Bitmap::BlackKnight, captureType, capturedIndex, movedIndex });
            }
        }
    }
}





std::string Chessboard::BitmapsToMove(unsigned long long startSquare, unsigned long long targetSquare) {
    std::string move;
    move.resize(4);
    char startIndex = 0;
    char targetIndex = 0;
    while (startSquare != 1)
    {
        startSquare = startSquare >> 1;
        startIndex++;
    }
    while (targetSquare != 1)
    {
        targetSquare = targetSquare >> 1;
        targetIndex++;
    }
    move[0] = startIndex % 8 + 'a';
    move[1] = startIndex / 8 + '1';
    move[2] = targetIndex % 8 + 'a';
    move[3] = targetIndex / 8 + '1';
    return move;
}

void Chessboard::MakeMove(Move move) {
    Bitmaps[move.PieceType] = Bitmaps[move.PieceType] ^ move.StartSquare | move.TargetSquare;
    if (((((move.CaptureType == Bitmap::BlackPawn) && ((Bitmaps[Bitmap::BlackPawn] & move.TargetSquare) == 0))) || ((move.CaptureType == Bitmap::WhitePawn) && ((Bitmaps[Bitmap::BlackPawn] & move.TargetSquare) == 0))))
    {
        if (move.CaptureType == Bitmap::WhitePawn)
        {
            Bitmaps[move.CaptureType] ^= (move.TargetSquare >> 8);
        }
        else
        {
            Bitmaps[move.CaptureType] ^= (move.TargetSquare << 8);
        }
    }
    else if (move.CaptureType != -1)
    {
        Bitmaps[move.CaptureType] ^= (move.TargetSquare);
        pieceLocations[move.CaptureType].erase(pieceLocations[move.CapturedIndex].begin() + move.CapturedIndex);
    }
    enPassantTarget = move.EnPassantTarget;
    if (moveSide == 0)
    {
        moveSide = 1;
    }
    else
    {
        moveSide = 0;
    }
    //Add Moves that get rid of castling rights as well
    if ((move.PieceType != Bitmap::WhitePawn) && (move.PieceType != Bitmap::BlackPawn) && move.CaptureType != NULL)
    {
        halfMoveClock++;
    }
    else
    {
        halfMoveClock = NULL;
    }
    if (move.PieceType >= Bitmap::BlackPawn)
    {
        fullMoveClock++;
    }
    LegalMoves.clear();
    Bitmaps[Bitmap::WhiteFull] = Bitmaps[Bitmap::WhitePawn] | Bitmaps[Bitmap::WhiteBishop] | Bitmaps[Bitmap::WhiteKnight] | Bitmaps[Bitmap::WhiteRook] | Bitmaps[Bitmap::WhiteQueen] | Bitmaps[Bitmap::WhiteKing];
    Bitmaps[Bitmap::BlackFull] = Bitmaps[Bitmap::BlackPawn] | Bitmaps[Bitmap::BlackBishop] | Bitmaps[Bitmap::BlackKnight] | Bitmaps[Bitmap::BlackRook] | Bitmaps[Bitmap::BlackQueen] | Bitmaps[Bitmap::BlackKing];
    Bitmaps[Bitmap::Full] = Bitmaps[Bitmap::WhiteFull] | Bitmaps[Bitmap::BlackFull];
}


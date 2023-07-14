#include "ChessBoard.h"
#include <iostream>
#include <bitset>

namespace Bitmap {
    enum Bitmap : int {
        WhitePawn = 0,
        WhiteKnight = 1,
        WhiteBishop = 2,
        WhiteRook = 3,
        WhiteQueen = 4,
        WhiteKing = 5,
        WhiteFull = 6,

        BlackPawn = 7,
        BlackKnight = 8,
        BlackBishop = 9,
        BlackRook = 10,
        BlackQueen = 11,
        BlackKing = 12,
        BlackFull = 13,
    };
}

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
    enPassantTarget = "";
    kingIndex = "e1";
    for (int i = 0; i < 14; i++)
    {
        Bitmaps[i] = 0UL;
    }
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
    /*
    std::cout << "White Pawn: " << std::bitset<64>(Bitmaps[Bitmap::WhitePawn]) << std::endl;
    std::cout << "White Bishop: " << std::bitset<64>(Bitmaps[Bitmap::WhiteBishop]) << std::endl;
    std::cout << "White Knight: " << std::bitset<64>(Bitmaps[Bitmap::WhiteKnight]) << std::endl;
    std::cout << "White Rook: " << std::bitset<64>(Bitmaps[Bitmap::WhiteRook]) << std::endl;
    std::cout << "White Queen: " << std::bitset<64>(Bitmaps[Bitmap::WhiteQueen]) << std::endl;
    std::cout << "White King: " << std::bitset<64>(Bitmaps[Bitmap::WhiteKing]) << std::endl;
    std::cout << "Black Pawn: " << std::bitset<64>(Bitmaps[Bitmap::BlackPawn]) << std::endl;
    std::cout << "Black Bishop: " << std::bitset<64>(Bitmaps[Bitmap::BlackBishop]) << std::endl;
    std::cout << "Black Knight: " << std::bitset<64>(Bitmaps[Bitmap::BlackKnight]) << std::endl;
    std::cout << "Black Rook: " << std::bitset<64>(Bitmaps[Bitmap::BlackRook]) << std::endl;
    std::cout << "Black Queen: " << std::bitset<64>(Bitmaps[Bitmap::BlackQueen]) << std::endl;
    std::cout << "Black King: " << std::bitset<64>(Bitmaps[Bitmap::BlackKing]) << std::endl;
    */

    //Skip over space
    index++;

    //Set moveside to moveside
    moveSide = FEN[index];

    //Skip to castling section
    index += 2;

    //Variable to determine where in the castle portion of the FEN code we are 0 for White King Side 1 for White Queen Side etc.
    int castlingIndex = 0;

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
        castlingIndex++;
    }
    index++;

    //Get en-passant target square
    while (FEN[index] != ' ') {
        enPassantTarget += FEN[index];
        index++;
    }
    index++;

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





#pragma once
#include <vector>
#include <string>

class Chessboard {
    
public:
    std::vector<std::vector<char>> board;
    char moveSide = '-';
    char castlingAbility[4] = { '-', '-', '-', '-' };
    std::string enPassantTarget = "";
    int halfMoveClock = 0;
    int fullMoveClock = 0;
    std::string kingIndex = "e1";
    // Constructor
    Chessboard() {
        board.resize(8, std::vector<char>(8, '-'));
    }

    // Getter for accessing elements using the syntax VariableName[n][n]
    std::vector<char>& operator[](int index) {
        return board[index];
    }
};
Chessboard FENtoChessboard(std::string FEN) {
    Chessboard FENBoard;
    short column = 0;
    short row = 7;
    short index = 0;

    //Set board to FEN position
    while(FEN[index] != ' ')
    {
        if (column > 7)
        {
            row -= 1;
            column -= 8;
        }
        if (FEN[index] > '0' && FEN[index] <= '9')
        {
            column += FEN[index] - '0';
        }
        else if (FEN[index] != '/')
        {
            FENBoard[column][row] = FEN[index];
            column++;
        }
        index++;
    }
    //To skip over space
    index++;

    //Set moveside to moveside
    FENBoard.moveSide = FEN[index];

    //Skip to castling section
    index += 2;

    //Variable to determine where in the castle portion of the FEN code we are 0 for White King Side 1 for White Queen Side etc.
    int castlingIndex = 0;

    //Loop for castling ability
    while(FEN[index] != ' ')
    {
        if (FEN[index] == 'K')
        {
            FENBoard.castlingAbility[0] = FEN[index];
        }
        if (FEN[index] == 'Q')
        {
            FENBoard.castlingAbility[1] = FEN[index];
        }
        if (FEN[index] == 'k')
        {
            FENBoard.castlingAbility[2] = FEN[index];
        }
        if (FEN[index] == 'q')
        {
            FENBoard.castlingAbility[3] = FEN[index];
        }
        index++;
        castlingIndex++;
    }
    index++;

    //Get en-passant target square
    while(FEN[index] != ' ') {
        FENBoard.enPassantTarget += FEN[index];
        index++;
    }
    index++;

    //Get half move count
    int halfMoveDigits = -1;
    int tempIndex = index;
    while (FEN[tempIndex] != ' '){
        halfMoveDigits++;
        tempIndex++;
    }
    while (FEN[index] != ' ')
    {
        FENBoard.halfMoveClock += (FEN[index] - '0') * pow(10, halfMoveDigits);
        halfMoveDigits--;
        index++;
    }
    index++;
    //Get full move count
    int fullMoveDigits = FEN.length() - index - 1;
    while (index < FEN.length())
    {
        FENBoard.fullMoveClock += (FEN[index] - '0') * pow(10, fullMoveDigits);
        fullMoveDigits--;
        index++;
    }
    return FENBoard;
}

short SquareToRow(std::string Square){
    return Square[1] - '0';
}

short SquareToColumn(std::string Square){
    return Square[0] - 'a' + 1;
}
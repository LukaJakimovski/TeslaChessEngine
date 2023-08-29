#include "Move.h"
#include "Enums.h"
#include "ChessBoard.h"
char moveStrings[64][3] = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3","a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

std::string Move::GetMoveString(int startIndex, int targetIndex) {
	std::string MoveString(5, '\0');
	MoveString[0] = moveStrings[startIndex][0];
	MoveString[1] = moveStrings[startIndex][1];
	MoveString[2] = moveStrings[targetIndex][0];
	MoveString[3] = moveStrings[targetIndex][1];
	MoveString[4] = '\0';
	return MoveString;
}

std::string Move::GetMoveString(int startIndex, int targetIndex, int promotionType) {
	std::string MoveString = GetMoveString(startIndex, targetIndex);
	if (promotionType == Bitmap::WhiteKnight || promotionType == Bitmap::BlackKnight)
		MoveString[4] = 'n';
	else if (promotionType == Bitmap::WhiteBishop || promotionType == Bitmap::BlackBishop)
		MoveString[4] = 'b';
	else if (promotionType == Bitmap::WhiteRook || promotionType == Bitmap::BlackRook)
		MoveString[4] = 'r';
	else if (promotionType == Bitmap::WhiteQueen || promotionType == Bitmap::BlackQueen)
		MoveString[4] = 'q';
	return MoveString;
}

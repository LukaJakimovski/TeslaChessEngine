#include "Move.h"
#include "Enums.h"
#include "ChessBoard.h"

char moveStrings[64][3] = { "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3","a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8" };

std::string Move::GetMoveString(int startIndex, int targetIndex) {
	std::string MoveString(5, '\0');
	MoveString[0] = moveStrings[Chessboard::BitmapToBitindex(startIndex)][0];
	MoveString[1] = moveStrings[Chessboard::BitmapToBitindex(startIndex)][1];
	MoveString[2] = moveStrings[Chessboard::BitmapToBitindex(targetIndex)][0];
	MoveString[3] = moveStrings[Chessboard::BitmapToBitindex(targetIndex)][1];
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

std::string Move::GetMoveString() {
	std::string MoveString(5, '\0');
	MoveString[0] = moveStrings[Chessboard::BitmapToBitindex(StartSquare)][0];
	MoveString[1] = moveStrings[Chessboard::BitmapToBitindex(StartSquare)][1];
	MoveString[2] = moveStrings[Chessboard::BitmapToBitindex(TargetSquare)][0];
	MoveString[3] = moveStrings[Chessboard::BitmapToBitindex(TargetSquare)][1];
	MoveString[4] = '\0';
	if (PromotionType == Bitmap::WhiteKnight || PromotionType == Bitmap::BlackKnight)
		MoveString[4] = 'n';
	else if (PromotionType == Bitmap::WhiteBishop || PromotionType == Bitmap::BlackBishop)
		MoveString[4] = 'b';
	else if (PromotionType == Bitmap::WhiteRook || PromotionType == Bitmap::BlackRook)
		MoveString[4] = 'r';
	else if (PromotionType == Bitmap::WhiteQueen || PromotionType == Bitmap::BlackQueen)
		MoveString[4] = 'q';
	return MoveString;
}
std::string Move::GetLongMoveString(int startIndex, int targetIndex, int pieceType, int promotionType) {
	std::string MoveString(5, '\0');
	int stringStartIndex = 0;
	if (pieceType != Bitmap::WhitePawn && pieceType != Bitmap::BlackPawn)
	{
		if (pieceType == Bitmap::WhiteKnight || pieceType == Bitmap::BlackKnight)
			MoveString[0] = 'N';
		else if (pieceType == Bitmap::WhiteBishop || pieceType == Bitmap::BlackBishop)
			MoveString[0] = 'B';
		else if (pieceType == Bitmap::WhiteRook || pieceType == Bitmap::BlackRook)
			MoveString[0] = 'R';
		else if (pieceType == Bitmap::WhiteQueen || pieceType == Bitmap::BlackQueen)
			MoveString[0] = 'Q';
		else if (pieceType == Bitmap::WhiteKing || pieceType == Bitmap::BlackKing)
			MoveString[0] = 'K';
		stringStartIndex = 1;
	}
	MoveString[0 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(startIndex)][0];
	MoveString[1 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(startIndex)][1];
	MoveString[2 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(targetIndex)][0];
	MoveString[3 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(targetIndex)][1];
	if (promotionType == Bitmap::WhiteKnight || promotionType == Bitmap::BlackKnight)
		MoveString[4] = 'n';
	else if (promotionType == Bitmap::WhiteBishop || promotionType == Bitmap::BlackBishop)
		MoveString[4] = 'b';
	else if (promotionType == Bitmap::WhiteRook || promotionType == Bitmap::BlackRook)
		MoveString[4] = 'r';
	else if (promotionType == Bitmap::WhiteQueen || promotionType == Bitmap::BlackQueen)
		MoveString[4] = 'q';
	else
		MoveString.resize(4);
	return MoveString;
}

std::string Move::GetLongMoveString() {
	std::string MoveString(5, '\0');
	int stringStartIndex = 0;
	if (PieceType != Bitmap::WhitePawn && PieceType != Bitmap::BlackPawn)
	{
		if (PieceType == Bitmap::WhiteKnight || PieceType == Bitmap::BlackKnight)
			MoveString[0] = 'N';
		else if (PieceType == Bitmap::WhiteBishop || PieceType == Bitmap::BlackBishop)
			MoveString[0] = 'B';
		else if (PieceType == Bitmap::WhiteRook || PieceType == Bitmap::BlackRook)
			MoveString[0] = 'R';
		else if (PieceType == Bitmap::WhiteQueen || PieceType == Bitmap::BlackQueen)
			MoveString[0] = 'Q';
		else if (PieceType == Bitmap::WhiteKing || PieceType == Bitmap::BlackKing)
			MoveString[0] = 'K';
		stringStartIndex = 1;
	}
	MoveString[0 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(StartSquare)][0];
	MoveString[1 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(StartSquare)][1];
	MoveString[2 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(TargetSquare)][0];
	MoveString[3 + stringStartIndex] = moveStrings[Chessboard::BitmapToBitindex(TargetSquare)][1];
	if (PromotionType == Bitmap::WhiteKnight || PromotionType == Bitmap::BlackKnight)
		MoveString[4] = 'N';
	else if (PromotionType == Bitmap::WhiteBishop || PromotionType == Bitmap::BlackBishop)
		MoveString[4] = 'B';
	else if (PromotionType == Bitmap::WhiteRook || PromotionType == Bitmap::BlackRook)
		MoveString[4] = 'R';
	else if (PromotionType == Bitmap::WhiteQueen || PromotionType == Bitmap::BlackQueen)
		MoveString[4] = 'Q';
	else if (!(stringStartIndex))
		MoveString.resize(4);
	return MoveString;
}

Move::Move() {
	u64 StartSquare = 0;
	u64 TargetSquare = 0;
	int PieceType = 0;
	int CaptureType = 0;
	int PromotionType = 0;
	int MoveGuessScore = 0;
}


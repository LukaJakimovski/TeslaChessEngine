#include "Move.h"
#include "ChessBoard.h"

Move::Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, int pieceType, int captureType, int capturedIndex, int movedIndex) {
	MoveString = moveString;
	StartSquare = startSquare;
	TargetSquare = targetSquare;
	EnPassantTarget = enPassantTarget;
	PieceType = pieceType;
	CaptureType = captureType;
	CapturedIndex = capturedIndex;
	MovedIndex = movedIndex;
	PromotionType = 0;
}

Move::Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, int pieceType, int captureType, int capturedIndex, int movedIndex, int promotionType) {
	MoveString = moveString;
	StartSquare = startSquare;
	TargetSquare = targetSquare;
	EnPassantTarget = enPassantTarget;
	PieceType = pieceType;
	CaptureType = captureType;
	CapturedIndex = capturedIndex;
	MovedIndex = movedIndex;
	PromotionType = promotionType;
}
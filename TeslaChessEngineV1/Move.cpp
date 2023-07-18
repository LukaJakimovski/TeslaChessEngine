#include "Move.h"
#include "ChessBoard.h"

Move::Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, short pieceType, short captureType, short capturedIndex, short movedIndex) {
	MoveString = moveString;
	StartSquare = startSquare;
	TargetSquare = targetSquare;
	EnPassantTarget = enPassantTarget;
	PieceType = pieceType;
	CaptureType = captureType;
	CapturedIndex = capturedIndex;
	MovedIndex = movedIndex;
}

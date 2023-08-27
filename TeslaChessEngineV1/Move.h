#pragma once
#include <string>
class Move
{	
public:
	std::string MoveString;
	unsigned long long StartSquare;
	unsigned long long TargetSquare;
	unsigned long long EnPassantTarget;
	int PieceType;
	int CaptureType;
	int CapturedIndex;
	int MovedIndex;
	int PromotionType;
	Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget,  int pieceType, int captureType, int capturedIndex, int movedIndex);
	Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, int pieceType, int captureType, int capturedIndex, int movedIndex, int promotionType);
	Move();
};


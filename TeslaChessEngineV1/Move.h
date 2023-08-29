#pragma once
#include <string>
class Move
{
public:
	//char* MoveString = new char[5];
	unsigned long long StartSquare;
	unsigned long long TargetSquare;
	unsigned long long EnPassantTarget;
	int PieceType;
	int CaptureType;
	int PromotionType;
	Move(unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, int pieceType, int captureType);
	Move(unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget, int pieceType, int captureType, int promotionType);
	Move();
	std::string GetMoveString(int startIndex, int targetIndex, int promotionType);
	std::string GetMoveString(int startIndex, int targetIndex);
};


#pragma once
#include <string>
class Move
{
public:
	unsigned long long StartSquare;
	unsigned long long TargetSquare;
	unsigned long long EnPassantTarget;
	int PieceType;
	int CaptureType;
	int PromotionType;
	std::string GetMoveString(int startIndex, int targetIndex, int promotionType);
	std::string GetMoveString(int startIndex, int targetIndex);
};


#pragma once
#include <string>

using u64 = unsigned long long;
class Move
{
public:
	u64 StartSquare;
	u64 TargetSquare;
	int PieceType;
	int CaptureType;
	int PromotionType;
	std::string GetMoveString(int startIndex, int targetIndex, int promotionType);
	std::string GetMoveString(int startIndex, int targetIndex);
};


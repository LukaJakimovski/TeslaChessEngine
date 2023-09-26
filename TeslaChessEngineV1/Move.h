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
	static std::string GetMoveString(int startIndex, int targetIndex, int promotionType);
	static std::string GetMoveString(int startIndex, int targetIndex);
	static std::string GetLongMoveString(int startIndex, int targetIndex, int pieceType, int promotionType);
	std::string GetLongMoveString();
};


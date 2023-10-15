#pragma once
#include <string>

using u64 = unsigned long long;
class Move
{
public:
	Move();
	u64 StartSquare;
	u64 TargetSquare;
	int PieceType;
	int CaptureType;
	int PromotionType;
	int MoveScoreGuess;
	Move(u64 startSquare, u64 targetSquare, int pieceType, int captureType, int promotionType, int moveScoreGuess);
	static std::string GetMoveString(int startIndex, int targetIndex, int promotionType);
	static std::string GetMoveString(int startIndex, int targetIndex);
	std::string GetMoveString();
	static std::string GetLongMoveString(int startIndex, int targetIndex, int pieceType, int promotionType);
	std::string GetLongMoveString();
};


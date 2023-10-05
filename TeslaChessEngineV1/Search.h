#pragma once
#include "Eval.h"
#include "ChessBoard.h"
extern std::string evaluated;
class Search
{
	public:
		static Eval SearchPosition(Chessboard position, int depth, int alpha, int beta, bool maximizingPlayer);

};


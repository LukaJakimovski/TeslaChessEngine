#include "Eval.h"
#include "ChessBoard.h"
#include "Enums.h"
int Chessboard::Evaluate() {
	int eval = 0;
	int gameState = CheckGameState();;
	if (gameState == 0)
	{
		for (int i = 0; i < 14; i++)
		{
			eval += pieceCounts[i] * pieceValues[i];
		}
		return eval;
	}
	if (gameState == 1)
	{
		return INT32_MAX;
	}
	if (gameState == 2)
	{
		return INT32_MIN;
	}
	else
	{
		return 0;
	}
}

Eval::Eval(Move Move, int Eval) {
	move = Move;
	eval = Eval;
}

Eval::Eval(Move Move, int Eval, int Alpha, int Beta) {
	move = Move;
	eval = Eval; 
	alpha = Alpha;
	beta = Beta;
}

Eval::Eval() {
	eval = 0;
}



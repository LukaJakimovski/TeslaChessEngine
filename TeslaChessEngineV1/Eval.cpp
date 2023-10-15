#include "Eval.h"
#include "ChessBoard.h"
#include "Enums.h"

int Chessboard::Evaluate() {
	int eval = 0;
	int gameState = CheckGameState();;
	if (gameState == 0)
	{
		eval += CountPieces();
		eval += PieceLocations();
		return eval;
	}
	if (gameState == 1)
	{
		return INT32_MAX - 1000;
	}
	if (gameState == 2)
	{
		return INT32_MIN + 1000;
	}
	else
	{
		return 0;
	}
}

int Chessboard::CountPieces() {
	int eval = 0;
	for (int i = 0; i < 14; i++)
	{
		eval += pieceCounts[i] * pieceValues[i];
	}
	return eval;
};

int Chessboard::PieceLocations() {
	int eval = 0;
	for (int i = 0; i < 64; i++)
	{
		if (pieceLocations[i] >= 0 && pieceLocations[i] <= 6)
		{
			eval += pieceSquareValues[pieceLocations[i]][i];
		}
		else if (pieceLocations[i] >= 7 && pieceLocations[i] <= 12)
		{
			eval -= pieceSquareValues[pieceLocations[i] - 7][i];
		}
	}
	return eval;
}

Eval::Eval(Move Move, int Eval) {
	move = Move;
	eval = Eval;
}

Eval::Eval() {
	eval = 0;
	move = nullMove;
}



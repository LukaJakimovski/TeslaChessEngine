#include "Search.h"
#include "ChessBoard.h"
#include "Enums.h"
#include <algorithm>
#include <iostream>

Eval Search::SearchPosition(Chessboard position, int depth, int alpha, int beta, bool minimizingPlayer) {
	Eval eval;
	Chessboard tempBoard = position;
	Move movePlayed;
	if (depth == 0)
	{
		return { movePlayed, position.Evaluate() };
	}
	Move* TrueMoves = new Move[position.TrueLegalMovesSize];
	int trueSize = position.TrueLegalMovesSize;
	for (int i = 0; i < position.TrueLegalMovesSize; i++)
	{
		TrueMoves[i] = position.TrueLegalMoves[i];
	}
	if (minimizingPlayer == 0)
	{
		int maxEval = INT32_MIN;
		for (int i = 0; i < trueSize; i++)
		{
			position.MakeMove(TrueMoves[i]);
			position.FindAllMoves();
			if (position.TrueLegalMovesSize == 0)
			{
				return { TrueMoves[i], position.Evaluate()};
			}
			position.OrderMoves();
			Eval eval = SearchPosition(position, depth - 1, alpha, beta, true);
			position = tempBoard;
			maxEval = std::max(maxEval, eval.eval);
			if (eval.eval == maxEval)
			{
				movePlayed = TrueMoves[i];
			}
			alpha = std::max(alpha, eval.eval);
			if (beta <= alpha)
				break;

		}
		return { movePlayed, maxEval, alpha, beta };
	}
	else
	{
		int minEval = INT32_MAX;
		for (int i = 0; i < trueSize; i++)
		{
			position.MakeMove(TrueMoves[i]);
			position.FindAllMoves();
			if (position.TrueLegalMovesSize == 0)
			{
				return { movePlayed, position.Evaluate()};
			}
			position.OrderMoves();
			Eval eval = SearchPosition(position, depth - 1, alpha, beta, false);
			position = tempBoard;
			minEval = std::min(minEval, eval.eval);
			if (eval.eval == minEval)
			{
				movePlayed = TrueMoves[i];
			}
			beta = std::min(beta, eval.eval);
			if (beta <= alpha) 
				break;
		}
		return { movePlayed, minEval, alpha, beta };
	}
};

void Chessboard::OrderMoves() {
	int offSet = moveSide * 7;
	for (int i = 0; i < TrueLegalMovesSize; i++)
	{
		TrueLegalMoves[i].MoveScoreGuess = 0;
		//Prioritise capturing opponent's valuable pieces with our less valuable ones
		if (TrueLegalMoves[i].CaptureType != -1)
		{
			TrueLegalMoves[i].MoveScoreGuess = 10 * pieceValues[TrueLegalMoves[i].CaptureType] - pieceValues[TrueLegalMoves[i].PieceType];
		}
		// Promoting a pawn is likely to be good
		if (TrueLegalMoves[i].PromotionType != 0)
		{
			TrueLegalMoves[i].MoveScoreGuess += pieceValues[TrueLegalMoves[i].PromotionType];
		}
		//Penalize moving outr piece attacked by an enemy pawn
		if (attackedSquares[Bitmap::WhitePawn + offSet] & TrueLegalMoves[i].TargetSquare)
		{
			TrueLegalMoves[i].MoveScoreGuess -= pieceValues[TrueLegalMoves[i].PieceType];
		}
	}
	std::sort(TrueLegalMoves, TrueLegalMoves+TrueLegalMovesSize, [](const Move& a, const Move& b) {
		return a.MoveScoreGuess > b.MoveScoreGuess;
	});
}
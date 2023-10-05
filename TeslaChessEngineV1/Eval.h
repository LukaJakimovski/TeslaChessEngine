#pragma once
#include "Move.h"
struct Eval
{
	Eval(Move Move, int Eval, int Alpha, int Beta);
	Eval(Move Move, int Eval);
	Eval();
	Move move;
	int eval;
	int alpha = INT32_MIN;
	int beta = INT32_MAX;
};

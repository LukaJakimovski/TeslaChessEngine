#pragma once
#include "Move.h"
struct Eval
{
	Eval(Move Move, int Eval);
	Eval();
	Move move;
	int eval;
	u64 nodes;
};

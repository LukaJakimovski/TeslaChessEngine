#pragma once
#include <string>
class Move
{	
public:
	std::string MoveString;
	unsigned long long StartSquare;
	unsigned long long TargetSquare;
	unsigned long long EnPassantTarget;
	short PieceType;
	short CaptureType;
	short CapturedIndex;
	short MovedIndex;

	Move(std::string moveString, unsigned long long startSquare, unsigned long long targetSquare, unsigned long long enPassantTarget,  short pieceType, short captureType, short capturedIndex, short movedIndex);


};


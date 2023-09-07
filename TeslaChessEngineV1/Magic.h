#pragma once
extern u64* rookBlockersMagics[64];
extern const u64 rookSightPremask[64];
extern const u64 rookSight[64];
extern const int RBits[64];
extern const int rookShifts[];
extern const u64 rookMagics[];

extern u64* bishopBlockersMagics[64];
extern const u64 bishopSightPremask[64];
extern const u64 bishopSight[64];
extern const int RBits[64];
extern const int bishopShifts[];
extern const u64 bishopMagics[];
//Random u64 number generator
unsigned long long randomGenerator();

u64* CreatePossibleRookBlockers(int squareIndex);

void MakeRookMagics(int squareIndex);

u64 FindSlidingRookMovesMagic(u64 blockers, u64 startIndex, u64 rowConstraint, u64 columnConstraint, int shiftAmount);

u64 CreateTargetSquareMapRook(u64 startSquare, u64 blockers);

void InitialiseMagicArrays();

u64* CreatePossibleBishopBlockers(int squareIndex);

void MakeBishopMagics(int squareIndex);

u64 FindSlidingBishopMovesMagic(u64 blockers, u64 startIndex, u64 rowConstraint, u64 columnConstraint, int shiftAmount);

u64 CreateTargetSquareMapBishop(u64 startSquare, u64 blockers);






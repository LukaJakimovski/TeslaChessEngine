#pragma once
namespace Bitmap {
    enum Bitmap : int {
        WhitePawn = 0,
        WhiteKnight = 1,
        WhiteBishop = 2,
        WhiteRook = 3,
        WhiteQueen = 4,
        WhiteKing = 5,
        WhiteFull = 6,

        BlackPawn = 7,
        BlackKnight = 8,
        BlackBishop = 9,
        BlackRook = 10,
        BlackQueen = 11,
        BlackKing = 12,
        BlackFull = 13,

        Full = 14
    };
}
namespace Row {
    enum Row : int {
        Row1 = 0,
        Row2 = 8,
        Row3 = 16,
        Row4 = 24,
        Row5 = 32,
        Row6 = 40,
        Row7 = 48,
        Row8 = 56,
    };
}
namespace Column {
    enum Column : unsigned long long {
        ColumnA = 0b0000000100000001000000010000000100000001000000010000000100000001,
        ColumnB = ColumnA << 1,
        ColumnC = ColumnB << 1,
        ColumnD = ColumnC << 1,
        ColumnE = ColumnD << 1,
        ColumnF = ColumnE << 1,
        ColumnG = ColumnF << 1,
        ColumnH = ColumnG << 1
    };
}
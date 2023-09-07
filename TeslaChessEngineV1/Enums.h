#pragma once
using u64 = unsigned long long;
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
    enum Row : u64 {
        Row1 = 0b0000000000000000000000000000000000000000000000000000000011111111,
        Row2 = Row1 << 8,
        Row3 = Row2 << 8,
        Row4 = Row3 << 8,
        Row5 = Row4 << 8,
        Row6 = Row5 << 8,
        Row7 = Row6 << 8,
        Row8 = Row7 << 8,
    };
}
namespace Column {
    enum Column : u64 {
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
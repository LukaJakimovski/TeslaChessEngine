#include <iostream>
#include <chrono>
#include "ChessBoard.h"
#include "Enums.h"
#include <bitset>
int globalDepth = 0;
int run = 0;
int index = 0;
std::vector<std::string> strings = {};
std::string toPrint = { };

unsigned long long Perft(int depth, Chessboard board)
{
    if (depth == 0) return 1;
    unsigned long long nodes = 0;
    std::vector<Move> moves;
    board.FindLegalMoves();
    board.FindAttackedSquares();
    board.FindTrueLegalMoves();
    board.FindCastlingMoves();
    moves = board.TrueLegalMoves;
    if (depth == 1 && globalDepth > 2) return moves.size();
    Chessboard boardCopy = board;
    if (depth == globalDepth && run == 0)
    {
        strings.resize(moves.size());
        for (int i = 0; i < moves.size(); i++)
        {
            strings[i] = moves[i].MoveString;
            run = 1;
            if (globalDepth == 1) {
            //std::cout << board.LegalMoves[i].MoveString << ' ';
            std::cout << board.TrueLegalMoves[i].MoveString << ": 1";
            std::cout << "\n";
            }
        }
    }
    for (int i = 0; i < moves.size(); i++)
    {
        board.MakeMove(moves[i]);
        nodes += Perft(depth - 1, board);
        board = boardCopy;
    }
    if (depth == globalDepth - 1) {
        std::cout << strings[index] << ": " << nodes << "\n";
        index++;
    }
    return nodes;
}
unsigned long long BasicPerft(int depth, Chessboard board) {
    if (depth == 0) return 1ULL;
    unsigned long long nodes = 0ULL;
    board.FindAllMoves();
    std::vector<Move> moves = board.TrueLegalMoves;
    if (depth == 1) return moves.size();
    Chessboard boardCopy = board;
    for (int i = 0; i < moves.size(); i++)
    {
        board.MakeMove(moves[i]);
        nodes += BasicPerft(depth - 1, board);
        board = boardCopy;
    }
    return nodes;
}
int main()
{
    int depth = 4;
    unsigned long long nodes = 0;
    unsigned long long expected = 0;
    globalDepth = depth;
    Chessboard board;
    std::string position;
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto totalStart = std::chrono::high_resolution_clock::now();
    auto totalEnd = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 Depth 5
    //r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 Depth 4
    //8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 Depth 6
    //r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 Depth 5
    //rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 Depth 4
    //r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 Depth 4
    totalStart = std::chrono::high_resolution_clock::now();
    //Position 1
    depth = 5;
    expected = 4865609;
    globalDepth = depth;
    position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.LoadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";

    //Position 2
    depth = 4;
    expected = 4085603;
    globalDepth = depth;
    position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
    board.LoadPosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";

    //Position 3
    depth = 6;
    expected = 11030083;
    globalDepth = depth;
    position = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
    board.LoadPosition("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";

    //Position 4
    depth = 5;
    expected = 15833292;
    globalDepth = depth;
    position = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    board.LoadPosition("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";

    //Position 5
    depth = 4;
    expected = 2103487;
    globalDepth = depth;
    position = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    board.LoadPosition("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";

    //Position 6
    depth = 4;
    expected = 3894594;
    globalDepth = depth;
    position = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
    board.LoadPosition("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    start = std::chrono::high_resolution_clock::now();
    nodes = BasicPerft(depth, board);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Nodes Counted: " << nodes << " Expected Nodes: " << expected << " Position FEN: " << position << " Depth: " << depth << " Time Spent " << duration.count() << " microseconds";
    if (expected == nodes) std::cout << " PASSED\n";
    else std::cout << " FAILED\n";
    totalEnd = std::chrono::high_resolution_clock::now();
    totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(totalEnd - totalStart);
    std::cout << "\nTotal time elapsed: " << totalDuration.count() << " microseconds";
    std::cin >> nodes;
}

/*
Chessboard board2;
run = 0;
index = 0;
board2.LoadPosition("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1R1K b kq - 1 1");
board2.FindAllMoves();
//board2.MakeMove(board2.TrueLegalMoves[3]);
//board2.FindAllMoves();
/*
std::cout << "White Pawn:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhitePawn]) << std::endl;
std::cout << "White Pawn:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhitePawn]) << std::endl;
std::cout << "White Bishop:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteBishop]) << std::endl;
std::cout << "White Bishop:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteBishop]) << std::endl;
std::cout << "White Knight:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteKnight]) << std::endl;
std::cout << "White Knight:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteKnight]) << std::endl;
std::cout << "White Rook:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteRook]) << std::endl;
std::cout << "White Rook:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteRook]) << std::endl;
std::cout << "White Queen:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteQueen]) << std::endl;
std::cout << "White Queen:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteQueen]) << std::endl;
std::cout << "White King:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteKing]) << std::endl;
std::cout << "White King:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteKing]) << std::endl;
std::cout << "Full White:\t" << std::bitset<64>(board.Bitmaps[Bitmap::WhiteFull]) << std::endl;
std::cout << "Full White:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::WhiteFull]) << std::endl;
std::cout << "Black Pawn:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackPawn]) << std::endl;
std::cout << "Black Pawn:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackPawn]) << std::endl;
std::cout << "Black Bishop:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackBishop]) << std::endl;
std::cout << "Black Bishop:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackBishop]) << std::endl;
std::cout << "Black Knight:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackKnight]) << std::endl;
std::cout << "Black Knight:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackKnight]) << std::endl;
std::cout << "Black Rook:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackRook]) << std::endl;
std::cout << "Black Rook:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackRook]) << std::endl;
std::cout << "Black Queen:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackQueen]) << std::endl;
std::cout << "Black Queen:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackQueen]) << std::endl;
std::cout << "Black King:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackKing]) << std::endl;
std::cout << "Black King:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackKing]) << std::endl;
std::cout << "Full Black:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::BlackFull]) << std::endl;
std::cout << "Full Black:\t" << std::bitset<64>(board.Bitmaps[Bitmap::BlackFull]) << std::endl;
std::cout << "Full:\t" << std::bitset<64>(board2.Bitmaps[Bitmap::Full]) << std::endl;
std::cout << "Full:\t" << std::bitset<64>(board.Bitmaps[Bitmap::Full]) << std::endl;

for (int i = 0; i < board.TrueLegalMoves.size(); i++)
{
    if (board.TrueLegalMoves[i].CaptureType != board2.TrueLegalMoves[i].CaptureType)
    {
        std::cout << "Capture Type " << i << ": " << board.TrueLegalMoves[i].CaptureType << "\n";
        std::cout << "Capture Type " << i << ": " << board2.TrueLegalMoves[i].CaptureType << "\n";
    }
}
//std::cout << "\nNodes Searched: " << Perft(depth, board2) << "\n\n";



/*
std::cout << "The Pseudo Legal Moves in this position are: ";
for (int i = 0; i < board.LegalMoves.size(); i++)
{
    std::cout << board.LegalMoves[i].MoveString << ' ';
    moveCount++;
}
std::cout << "\nThere are " << moveCount << " Pseudo Legal Moves";
moveCount = 0;


std::cout << "\n\nThe True Legal Moves in this position are: ";
for (int i = 0; i < board.TrueLegalMoves.size(); i++)
{
    std::cout << board.TrueLegalMoves[i].MoveString << ' ';
    moveCount++;
}
std::cout << "\nThere are " << moveCount << " True Legal Moves\n\n\n";
if (board.checkmate == 1)
{
    std::cout << "\n\nCHECKMATE\n\n";
}
else if (board.stalemate == 1)
{
    std::cout << "\n\nSTALEMATE\n\n";
}
*/
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

unsigned long long DividePerft(int depth, Chessboard board)
{
    if (depth == 0) return 1;
    unsigned long long nodes = 0;
    board.FindAllMoves();
    Move* TrueMoves = new Move[board.TrueLegalMovesSize];
    int trueSize = board.TrueLegalMovesSize;
    for (int i = 0; i < board.TrueLegalMovesSize; i++)
    {
        TrueMoves[i] = board.TrueLegalMoves[i];
    }
    if (depth == 1 && globalDepth > 2) return board.TrueLegalMovesSize;
    Chessboard boardCopy = board;
    if (depth == globalDepth && run == 0)
    {
        strings.resize(trueSize);
        run = 1;
        for (int i = 0; i < trueSize; i++)
        {
            strings[i] = board.TrueLegalMoves[i].GetMoveString(board.TrueLegalMoves[i].StartSquare, board.TrueLegalMoves[i].TargetSquare, board.TrueLegalMoves[i].PromotionType);

            if (globalDepth == 1) {
                //std::cout << board.LegalMoves[i].MoveString << ' ';
                std::cout << board.TrueLegalMoves[i].GetMoveString(board.TrueLegalMoves[i].StartSquare, board.TrueLegalMoves[i].TargetSquare, board.TrueLegalMoves[i].PromotionType) << ": 1";
                std::cout << "\n";
            }
        }
    }
    for (int i = 0; i < trueSize; i++)
    {
        board.MakeMove(TrueMoves[i]);
        nodes += DividePerft(depth - 1, board);
        board = boardCopy;
    }
    if (depth == globalDepth - 1) {
        std::cout << strings[index] << ": " << nodes << "\n";
        index++;
    }
    return nodes;
}
unsigned long long BasicPerft(int depth, Chessboard board) {
    Chessboard boardCopy = board;
    board.FindAllMoves();
    Move* TrueMoves = new Move[board.TrueLegalMovesSize];
    int trueSize = board.TrueLegalMovesSize;
    for (int i = 0; i < board.TrueLegalMovesSize; i++)
    {
        TrueMoves[i] = board.TrueLegalMoves[i];
    }
    if (depth == 1) return board.TrueLegalMovesSize;
    unsigned long long nodes = 0ULL;
    for (int i = 0; i < trueSize; i++)
    {
        board.MakeMove(TrueMoves[i]);
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
    Chessboard board2;
    std::string position;
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto totalStart = std::chrono::high_resolution_clock::now();
    auto totalEnd = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    totalStart = std::chrono::high_resolution_clock::now();

    //Position 1
    depth = 5;
    expected = 4865609;
    globalDepth = depth;
    position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 2";
    board.LoadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 2");
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
    board.LoadPosition(position);
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
    board.LoadPosition(position);
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
    board.LoadPosition(position);
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
    board.LoadPosition(position);
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
    board.LoadPosition(position);
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
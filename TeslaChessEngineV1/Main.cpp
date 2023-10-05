#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "ChessBoard.h"
#include "Enums.h"
#include "Magic.h"
#include "Search.h"

using u64 = unsigned long long;
int globalDepth = 0;
int run = 0;
int index = 0;
std::vector<std::string> strings = {};
std::string toPrint = { };

u64 DividePerft(int depth, Chessboard board)
{
    if (depth == 0) return 1;
    board.FindAllMoves();
    if (depth == 1 && globalDepth > 2) return board.TrueLegalMovesSize;
    u64 nodes = 0;
    Move* TrueMoves = new Move[board.TrueLegalMovesSize];
    int trueSize = board.TrueLegalMovesSize;
    for (int i = 0; i < board.TrueLegalMovesSize; i++)
    {
        TrueMoves[i] = board.TrueLegalMoves[i];
    }
    Chessboard boardCopy = board;
    if (depth == globalDepth && run == 0)
    {
        strings.resize(trueSize);
        run = 1;
        for (int i = 0; i < trueSize; i++)
        {
            strings[i] = board.TrueLegalMoves[i].GetMoveString();

            if (globalDepth == 1) {
                std::cout << board.TrueLegalMoves[i].GetMoveString() << ": 1";
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
    delete[] TrueMoves;
    return nodes;
}

u64 BasicPerft(int depth, Chessboard board) {
    board.FindAllMoves();
    if (depth == 1) return board.TrueLegalMovesSize;
    Chessboard boardCopy = board;

    Move* TrueMoves = new Move[board.TrueLegalMovesSize];
    int trueSize = board.TrueLegalMovesSize;
    for (int i = 0; i < board.TrueLegalMovesSize; i++)
    {
        TrueMoves[i] = board.TrueLegalMoves[i];
    }
    u64 nodes = 0ULL;
    for (int i = 0; i < trueSize; i++)
    {
        board.MakeMove(TrueMoves[i]);
        nodes += BasicPerft(depth - 1, board);
        board = boardCopy;
    }
    delete[] TrueMoves;
    return nodes;
}

void MoveGenSpeedTest() {
    int depth = 3;
    u64 nodes = 0;
    u64 expected = 0;
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
    
    //Position 1
    depth = 5;
    expected = 4865609;
    globalDepth = depth;
    index = 0;
    run = 0;
    position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    board.LoadPosition(position);
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
    index = 0;
    run = 0;
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
    index = 0;
    run = 0;
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
    index = 0;
    run = 0;
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
    index = 0;
    run = 0;
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
    index = 0;
    run = 0;
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
}

int GenerateRandom(int lowerBound, int upperBound) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
    return distribution(mt);
}

std::string PlayGameAIAI(std::string startPosition) {
    Chessboard board;
    int moveIndex;
    Move move;
    std::string movePGN;
    std::string gamePGN;
    board.LoadPosition(startPosition);
    auto start = std::chrono::high_resolution_clock::now();
    while (true)
    {
        board.FindAllMoves();
        if (board.CheckGameState() != 0)
            break;
        move = Search::SearchPosition(board, 3, INT32_MIN, INT32_MAX, board.moveSide).move;
        movePGN = move.GetLongMoveString();
        gamePGN += movePGN + " ";
        board.MakeMove(move);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " microseconds\n";
    return gamePGN;
}

std::string PlayGame(std::string startPosition) {
    std::string GamePGN;
    std::string PlayerMove = "\0\0\0\0\0";
    int playerMoveIndex = -1;
    Chessboard board;
    board.LoadPosition(startPosition);
    while (true)
    {
        playerMoveIndex = -1;
        board.FindAllMoves();
        if (board.CheckGameState() != 0)
        {
            std::cout << "GAME OVER\n";
            break;
        }
        std::cout << board.GenerateBoardDisplay();
        while (playerMoveIndex == -1)
        {
            std::cin >> PlayerMove;
            playerMoveIndex = board.FindMoveIndexFromPGN(PlayerMove);
            if (playerMoveIndex == -1)
            {
                std::cout << "Invalid Move Try Again\n";
            }
        }
        GamePGN += PlayerMove + " ";
        board.MakeMove(board.TrueLegalMoves[playerMoveIndex]);
    }
    return GamePGN;
}

void MainLoop() {
    Chessboard board;
    std::string gameMode;
    int run = 1;
    while (run)
    {
        std::cout << "Select Mode\n1. Move Generation and Speed Test\n";
        std::cout << "2. Play game AI vs AI (Prints Game PGN To Console Which Can Be Inputted At https://lichess.org/paste To View The Game)\n";
        std::cout << "3. Play game using User Input (Format (Piece Type), (StartSquare), (TargetSquare), (Promotion Type)) Ex. e2e4 (Pawn E4) Ng1f3 (Knight F3), Ke1g1 (King Castles King Side) \n";
        std::cout << "4. Evaluate Current Position\n";
        std::cout << "5. Quit\n";
        std::cin >> gameMode;
        board.LoadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
        if (gameMode == "1")
        {
            MoveGenSpeedTest();
            break;
        }
        else if (gameMode == "2") {
            std::cout << PlayGameAIAI("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
            break;
        }
        else if (gameMode == "3") {
            std::cout << PlayGame("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
            break;
        }
        else if (gameMode == "4")
        {
            board.FindAllMoves();
            Eval eval = Search::SearchPosition(board, 7, INT32_MIN, INT32_MAX, board.moveSide);
            std::cout << eval.move.GetLongMoveString();
            std::cout << "\n" << eval.eval << "\n";
            break;
        }
        else if (gameMode == "5") {
            break;
        }
        else
        {
            std::cout << "Invalid Input\n";
        }
    }
}

int main()
{
    InitialiseMagicArrays();
    MainLoop();
}
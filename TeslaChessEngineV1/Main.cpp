#include <iostream>
#include <chrono>
#include <random>
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
    memcpy(TrueMoves, board.TrueLegalMoves, sizeof(Move) * board.TrueLegalMovesSize);
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

std::string PlayGameAIAI(std::string startPosition) {
    Chessboard board;
    int moveIndex;
    Move move;
    std::string movePGN;
    std::string gamePGN;
    board.LoadPosition(startPosition);
    auto start = std::chrono::high_resolution_clock::now();
    int index = 5;
    while (index != 0)
    {
        board.FindAllMoves();
        if (board.CheckGameState() != 0)
            break;
        move = Search::SearchPosition(board, index, INT32_MIN, INT32_MAX, board.moveSide).move;
        //index--;
        movePGN = move.GetLongMoveString();
        gamePGN += movePGN + " ";
        board.MakeMove(move);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " microseconds\n";
    return gamePGN;
}

std::string PlayGameVsAI(std::string startPosition) {
    Chessboard board;
    int moveIndex;
    int playerMoveIndex = -1;
    Move move;
    std::string PlayerMove = "\0\0\0\0\0";
    std::string movePGN;
    std::string gamePGN;
    board.LoadPosition(startPosition);
    auto start = std::chrono::high_resolution_clock::now();
    while (true)
    {
        //Player Moves
        playerMoveIndex = -1;
        board.FindAllMoves();
        //std::cout << board.GenerateBoardDisplay();
        if (board.CheckGameState() != 0)
            break;
        while (playerMoveIndex == -1)
        {
            std::cin >> PlayerMove;
            playerMoveIndex = board.FindMoveIndexFromPGN(PlayerMove);
            if (playerMoveIndex == -1)
            {
                std::cout << "Invalid Move Try Again\n";
            }
        }
        gamePGN += PlayerMove + " ";
        board.MakeMove(board.TrueLegalMoves[playerMoveIndex]);
        //AI Moves
        board.FindAllMoves();
        if (board.CheckGameState() != 0)
            break;
        move = Search::SearchPosition(board, 5, INT32_MIN, INT32_MAX, board.moveSide).move;
        movePGN = move.GetLongMoveString();
        std::cout << movePGN << "\n";
        gamePGN += movePGN + " ";
        board.MakeMove(move);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " microseconds\n";
    return gamePGN;
}

void SearchCheckmateFindTest() {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Expected Moves Per Position 5" << "\n";
    std::cout << PlayGameAIAI("r1b1kb1r/pppp1ppp/5q2/4n3/3KP3/2N3PN/PPP4P/R1BQ1B1R b kq - 0 1") << "\n";
    std::cout << PlayGameAIAI("r3k2r/ppp2Npp/1b5n/4p2b/2B1P2q/BQP2P2/P5PP/RN5K w kq - 1 0") << "\n";
    std::cout << PlayGameAIAI("r1b3kr/ppp1Bp1p/1b6/n2P4/2p3q1/2Q2N2/P4PPP/RN2R1K1 w - - 1 0") << "\n";
    std::cout << PlayGameAIAI("r2n1rk1/1ppb2pp/1p1p4/3Ppq1n/2B3P1/2P4P/PP1N1P1K/R2Q1RN1 b - - 0 1") << "\n";
    std::cout << PlayGameAIAI("3q1r1k/2p4p/1p1pBrp1/p2Pp3/2PnP3/5PP1/PP1Q2K1/5R1R w - - 1 0") << "\n";
    std::cout << PlayGameAIAI("6k1/ppp2ppp/8/2n2K1P/2P2P1P/2Bpr3/PP4r1/4RR2 b - - 0 1") << "\n";
    std::cout << PlayGameAIAI("rn3rk1/p5pp/2p5/3Ppb2/2q5/1Q6/PPPB2PP/R3K1NR b - - 0 1") << "\n";
    std::cout << PlayGameAIAI("N1bk4/pp1p1Qpp/8/2b5/3n3q/8/PPP2RPP/RNB1rBK1 b - - 0 1") << "\n";
    std::cout << PlayGameAIAI("8/2p3N1/6p1/5PB1/pp2Rn2/7k/P1p2K1P/3r4 w - - 1 0") << "\n";
    std::cout << PlayGameAIAI("r1b1k1nr/p2p1ppp/n2B4/1p1NPN1P/6P1/3P1Q2/P1P1K3/q5b1 w - - 1 0") << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << duration.count() << " microseconds\n";
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
        std::cout << "5. Position Search Test\n";
        std::cout << "6. Play Game Vs AI\n";
        std::cout << "7. Quit\n";
        std::cin >> gameMode;
        board.LoadPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
        if (gameMode == "1")
        {
            MoveGenSpeedTest();
        }
        else if (gameMode == "2") {
            std::cout << PlayGameAIAI("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0") << "\n";
        }
        else if (gameMode == "3") {
            std::cout << PlayGame("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0") << "\n";
        }
        else if (gameMode == "4")
        {
            board.FindAllMoves();
            Eval eval = Search::SearchPosition(board, 2, INT32_MIN, INT32_MAX, board.moveSide);
            std::cout << eval.move.GetLongMoveString();
            std::cout << "\n" << eval.eval << "\n";
        }
        else if (gameMode == "5") {
            SearchCheckmateFindTest();
        }
        else if (gameMode == "6") {
            std::cout << PlayGameVsAI("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0") << "\n";
        }
        else if (gameMode == "7")
        {
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
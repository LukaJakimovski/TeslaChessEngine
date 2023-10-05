#include <random>
#include <fstream>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include "ChessBoard.h"
#include "Enums.h"
#include "Magic.h"

using u64 = unsigned long long;
u64* rookBlockersMagics[64];
const u64 rookSightPremask[64] = { 72340172838076926, 144680345676153597, 289360691352306939, 578721382704613623, 1157442765409226991, 2314885530818453727, 4629771061636907199, 9259542123273814143, 72340172838141441, 144680345676217602, 289360691352369924, 578721382704674568, 1157442765409283856, 2314885530818502432, 4629771061636939584, 9259542123273813888, 72340172854657281, 144680345692602882, 289360691368494084, 578721382720276488, 1157442765423841296, 2314885530830970912, 4629771061645230144, 9259542123273748608, 72340177082712321, 144680349887234562, 289360695496279044, 578721386714368008, 1157442769150545936, 2314885534022901792, 4629771063767613504, 9259542123257036928, 72341259464802561, 144681423712944642, 289361752209228804, 578722409201797128, 1157443723186933776, 2314886351157207072, 4629771607097753664, 9259542118978846848, 72618349279904001, 144956323094725122, 289632270724367364, 578984165983651848, 1157687956502220816, 2315095537539358752, 4629910699613634624, 9259541023762186368, 143553341945872641, 215330564830528002, 358885010599838724, 645993902138460168, 1220211685215703056, 2368647251370188832, 4665518383679160384, 9259260648297103488, 18302911464433844481, 18231136449196065282, 18087586418720506884, 17800486357769390088, 17226286235867156496, 16077885992062689312, 13781085504453754944, 9187484529235886208 };
const u64 rookSight[64] = { 282578800148862, 565157600297596, 1130315200595066, 2260630401190006, 4521260802379886, 9042521604759646, 18085043209519166, 36170086419038334, 282578800180736, 565157600328704, 1130315200625152, 2260630401218048, 4521260802403840, 9042521604775424, 18085043209518592, 36170086419037696, 282578808340736, 565157608292864, 1130315208328192, 2260630408398848, 4521260808540160, 9042521608822784, 18085043209388032, 36170086418907136, 282580897300736, 565159647117824, 1130317180306432, 2260632246683648, 4521262379438080, 9042522644946944, 18085043175964672, 36170086385483776, 283115671060736, 565681586307584, 1130822006735872, 2261102847592448, 4521664529305600, 9042787892731904, 18085034619584512, 36170077829103616, 420017753620736, 699298018886144, 1260057572672512, 2381576680245248, 4624614895390720, 9110691325681664, 18082844186263552, 36167887395782656, 35466950888980736, 34905104758997504, 34344362452452352, 33222877839362048, 30979908613181440, 26493970160820224, 17522093256097792, 35607136465616896, 9079539427579068672, 8935706818303361536, 8792156787827803136, 8505056726876686336, 7930856604974452736, 6782456361169985536, 4485655873561051136, 9115426935197958144 };
const int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};
const int rookShifts[] = { 52, 53, 53, 53, 53, 53, 53, 52, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 53, 54, 54, 54, 54, 54, 54, 53, 52, 53, 53, 53, 53, 53, 53, 52 };
const u64 rookMagics[] = { 108091339932984400, 6359082811312309312, 9367496845934330018, 2954378952975908896, 144126183729795208, 9367504851545227281, 4647715365235736832, 36028936605452544, 29695611116741760, 36099440909492224, 1266706123104256, 1172202884117301248, 72339086479722498, 9235194003340132864, 4650248644289892864, 9532431582904746500, 708635449622560, 1153520739517792898, 2886844744846049792, 9404081171111878720, 13836184505012453504, 9148488176632320, 74524898147967504, 1154353069304086788, 9802225330757370944, 1157460289680068610, 1751900806952517680, 11533718787430289664, 587738447365013508, 1459201498020840496, 4612820731607253360, 140911434813696, 334526421139584, 9223513882478256194, 406485070145130500, 4521192367065347, 146375786138452992, 2306406852553869316, 14411527612335657091, 577103434600153281, 144256200579055616, 9948456112351100936, 9245327155376816192, 282643208536096, 290482314478355456, 144185626076905748, 4653493921853472770, 5765206759057260548, 18014684124816512, 295082506879744, 1738389662363453056, 4614237160455930368, 2252904292418816, 63191286890758272, 563259275215360, 216454265684656384, 3459910896434823682, 251823596273681, 9531939051081302277, 9245925425538926757, 144678157097115650, 20547682024292357, 288870296348299300, 20547691589173763 };

unsigned long long randomGenerator() {
    // Create a random number generator engine for 64-bit numbers
    std::random_device rd;
    std::mt19937_64 gen(rd());

    // Define a distribution for 64-bit integers
    std::uniform_int_distribution<unsigned long long> dist(0ULL, std::numeric_limits<unsigned long long>::max());

    // Generate and print a random 64-bit integer
    unsigned long long randomNum = dist(gen);
    return randomNum;
}
void Chessboard::CreateRookBitmaps() {
    std::ofstream rookBitmaps("rookBitmaps.txt");
    u64 attacked = 0;
    for (int i = 0; i < 64; i++)
    {
        attacked = 0;
        Bitmaps[Bitmap::WhiteRook] = 1ULL << i;
        FindRookMoves();
        for (int i = 0; i < LegalMovesSize; i++)
        {
            attacked |= LegalMoves[i].TargetSquare;
        }
        LegalMovesSize = 0;
        rookBitmaps << attacked << ", ";
        Bitmaps[Bitmap::WhiteRook] = 0;
    }
    rookBitmaps.close();
}

void Chessboard::CreateKingBitmaps() {
    std::ofstream kingBitmaps("kingBitmaps.txt");
    u64 attacked = 0;
    for (int i = 0; i < 64; i++)
    {
        attacked = 0;
        Bitmaps[Bitmap::WhiteKing] = 1ULL << i;
        FindKingMoves();
        for (int i = 0; i < LegalMovesSize; i++)
        {
            attacked |= LegalMoves[i].TargetSquare;
        }
        LegalMovesSize = 0;
        kingBitmaps << attacked << ", ";
    }
    kingBitmaps.close();
}

void Chessboard::CreateKnightBitmaps() {
    std::ofstream knightBitmaps("knightBitmaps.txt");
    u64 attacked = 0;
    for (int i = 0; i < 64; i++)
    {
        attacked = 0;
        Bitmaps[Bitmap::WhiteKnight] = 1ULL << i;
        FindKnightMoves();
        for (int i = 0; i < LegalMovesSize; i++)
        {
            attacked |= LegalMoves[i].TargetSquare;
        }
        LegalMovesSize = 0;
        knightBitmaps << attacked << ", ";
    }
    knightBitmaps.close();
}

void Chessboard::CreateBishopBitmaps() {
    std::ofstream bishopBitmaps("bishopBitmaps.txt");
    u64 attacked = 0;
    for (int i = 0; i < 64; i++)
    {
        attacked = 0;
        Bitmaps[Bitmap::WhiteBishop] = 1ULL << i;
        FindBishopMoves();
        for (int i = 0; i < LegalMovesSize; i++)
        {
            attacked |= LegalMoves[i].TargetSquare;
        }
        LegalMovesSize = 0;
        bishopBitmaps << attacked << ", ";
        Bitmaps[Bitmap::WhiteBishop] = 0;
    }
    bishopBitmaps.close();
}

// For rooks
u64* CreatePossibleRookBlockers(int squareIndex) {
    std::vector<int> shiftQuantity;
    u64 bitmap = 0;
    int maxSize = 0;

    for (int i = 0; i < 64; i++)
    {
        if ((rookSight[squareIndex] >> i) & 1)
        {
            shiftQuantity.push_back(i);
        }
    }
    maxSize = 1 << shiftQuantity.size();
    u64* rookAttack = new u64[maxSize];
    for (int i = 0; i < maxSize; i++)
    {
        for (int j = 0; j < shiftQuantity.size(); j++)
        {
            u64 bit = (i >> j) & 1;
            bitmap |= bit << shiftQuantity[j];
        }
        rookAttack[i] = bitmap;
        bitmap = 0ULL;
    }
    //rookAttack[2175];
    return rookAttack;
}

void MakeRookMagics(int squareIndex) {
    //Get Array Of Possible Blockers for current square
    u64* rookAttack = CreatePossibleRookBlockers(squareIndex);
    int lowestIndex = 32000;
    int lookedRands = 0;
    u64 bestMagic = 0;
    int bestShift = 0;
    int maxIndex = 0;
    int shiftAmount = 64 - RBits[squareIndex];
    u64 currentIndex = 0;
    int maxSize = 1 << RBits[squareIndex];
    std::unordered_map<int, int> lookUpTable;

    std::ofstream rookMagics("magics.txt", std::ios::app);
    std::ofstream rookShifts("shifts.txt", std::ios::app);
    std::ofstream rookMaxIndex("maxIndex.txt", std::ios::app);
    rookMagics.clear();
    rookShifts.clear();
    rookMaxIndex.clear();
    //Find Magic Number for current square
    while (true)
    {
        lookedRands++;
        if (lowestIndex == maxSize - 1)
        {
            std::cout << "Best Magic Found For Square " << squareIndex << ": " << bestMagic << " Shift Amount: " << bestShift << " Max Index " << lowestIndex << " Random Numbers Checked: " << lookedRands << "\n\n";
            rookMagics << bestMagic << ", ";
            rookShifts << bestShift << ", ";
            delete[] rookAttack;
            return;
        }
        maxIndex = 0;
        u64 randomNumber = randomGenerator() & randomGenerator() & randomGenerator();
        lookUpTable.clear();
        for (int blockerIndex = 0; blockerIndex < maxSize; blockerIndex++)
        {
            currentIndex = ((rookAttack[blockerIndex] * randomNumber) >> shiftAmount);
            if (lookUpTable[currentIndex] > 0)
            {
                lookUpTable.clear();
                break;
            }
            if (currentIndex > maxIndex) {
                maxIndex = currentIndex;
            }
            lookUpTable[currentIndex]++;
        }
        if ((lookUpTable.size() == maxSize) && (maxIndex < lowestIndex))
        {
            lowestIndex = maxIndex;
            bestMagic = randomNumber;
            bestShift = shiftAmount;
        }
    }
}

u64 FindSlidingRookMovesMagic(u64 blockers, u64 startIndex, u64 rowConstraint, u64 columnConstraint, int shiftAmount) {
    int negative = 0b10000000000000000000000000000000;
    int offSet = 0;
    int moveCount = 0;
    u64 tempStart = 0;
    u64 targetSquare = 0;
    u64 targetMap = 0;
    u64 startSquare = Chessboard::BitIndexToBitmap(startIndex);
    tempStart = startSquare;
    if ((shiftAmount & negative) == negative) targetSquare = startSquare >> (~shiftAmount + 1);
    else targetSquare = startSquare << shiftAmount;
    if (blockers == 0) return rookSightPremask[startIndex];
    while (true)
    {
        if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0) && ((targetSquare & ~(blockers)) == targetSquare))
        {
            targetMap |= targetSquare;
            moveCount++;
        }
        else if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0)) {
            targetMap |= targetSquare;
            break;
        }
        else
        {
            break;
        }
        tempStart = targetSquare;
        if ((shiftAmount & negative) == negative) targetSquare = targetSquare >> (~shiftAmount + 1);
        else targetSquare = targetSquare << shiftAmount;
    }
    return targetMap;
}

u64 CreateTargetSquareMapRook(u64 startSquare, u64 blockers) {
    u64 targetMap = 0;
    //North
    targetMap |= FindSlidingRookMovesMagic(blockers, startSquare, Row::Row8, NULL, 8);
    //West
    targetMap |= FindSlidingRookMovesMagic(blockers, startSquare, NULL, Column::ColumnA, -1);
    //East
    targetMap |= FindSlidingRookMovesMagic(blockers, startSquare, NULL, Column::ColumnH, 1);
    //South
    targetMap |= FindSlidingRookMovesMagic(blockers, startSquare, Row::Row1, NULL, -8);
    return targetMap;
}

u64* bishopBlockersMagics[64];
const u64 bishopSightPremask[64] = { 9241421688590303744, 36099303471056128, 141012904249856, 550848566272, 6480472064, 1108177604608, 283691315142656, 72624976668147712, 4620710844295151618, 9241421688590368773, 36099303487963146, 141017232965652, 1659000848424, 283693466779728, 72624976676520096, 145249953336262720, 2310355422147510788, 4620710844311799048, 9241421692918565393, 36100411639206946, 424704217196612, 72625527495610504, 145249955479592976, 290499906664153120, 1155177711057110024, 2310355426409252880, 4620711952330133792, 9241705379636978241, 108724279602332802, 145390965166737412, 290500455356698632, 580999811184992272, 577588851267340304, 1155178802063085600, 2310639079102947392, 4693335752243822976, 9386671504487645697, 326598935265674242, 581140276476643332, 1161999073681608712, 288793334762704928, 577868148797087808, 1227793891648880768, 2455587783297826816, 4911175566595588352, 9822351133174399489, 1197958188344280066, 2323857683139004420, 144117404414255168, 360293502378066048, 720587009051099136, 1441174018118909952, 2882348036221108224, 5764696068147249408, 11529391036782871041, 4611756524879479810, 567382630219904, 1416240237150208, 2833579985862656, 5667164249915392, 11334324221640704, 22667548931719168, 45053622886727936, 18049651735527937 };
const u64 bishopSight[64] = { 18049651735527936, 70506452091904, 275415828992, 1075975168, 38021120, 8657588224, 2216338399232, 567382630219776, 9024825867763712, 18049651735527424, 70506452221952, 275449643008, 9733406720, 2216342585344, 567382630203392, 1134765260406784, 4512412933816832, 9024825867633664, 18049651768822272, 70515108615168, 2491752130560, 567383701868544, 1134765256220672, 2269530512441344, 2256206450263040, 4512412900526080, 9024834391117824, 18051867805491712, 637888545440768, 1135039602493440, 2269529440784384, 4539058881568768, 1128098963916800, 2256197927833600, 4514594912477184, 9592139778506752, 19184279556981248, 2339762086609920, 4538784537380864, 9077569074761728, 562958610993152, 1125917221986304, 2814792987328512, 5629586008178688, 11259172008099840, 22518341868716544, 9007336962655232, 18014673925310464, 2216338399232, 4432676798464, 11064376819712, 22137335185408, 44272556441600, 87995357200384, 35253226045952, 70506452091904, 567382630219776, 1134765260406784, 2832480465846272, 5667157807464448, 11333774449049600, 22526811443298304, 9024825867763712, 18049651735527936 };
const int BBits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};
const int bishopShifts[] = { 58, 59, 59, 59, 59, 59, 59, 58, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 57, 57, 57, 57, 59, 59, 59, 59, 57, 55, 55, 57, 59, 59, 59, 59, 57, 55, 55, 57, 59, 59, 59, 59, 57, 57, 57, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 58, 59, 59, 59, 59, 59, 59, 58 };
const u64 bishopMagics[] = { 18067226612482116, 76633763648471556, 1159720889355354624, 230885355795649360, 73188580864294986, 6834573138825856, 720861836559254144, 38315931799658504, 6974176614062227713, 4618494355085230208, 74801186357378, 22011741997056, 9108555300864, 6631530729472, 652902277121, 722941007104442896, 20548635407172104, 1450722133088403984, 578714803757975040, 9250428827748810756, 577024818958041096, 844459575119888, 567349326383104, 2896096173362775044, 4910194904692953120, 4512397952288944, 289396015377695744, 2260596982554656, 18859098384908296, 281749989361665, 613898163639715880, 4620765789929149712, 14649948765167712, 596731419540783616, 937876834442413056, 45038206034903168, 2254016016814336, 9016004071932160, 4504703434096768, 20338784344244552, 1155175709686108160, 1726302008656897, 41097559111893505, 1134979476621312, 22520609644746496, 2540078035370512, 2455825354548315136, 73750859132971552, 1153563705565792256, 565151158247552, 9018271831689984, 4612248979186124032, 3404225508017152, 76570128406315104, 1170944733594420484, 595618694449627136, 72343468151935010, 1201225103058048, 6953698567555125248, 72198881303463937, 1416222785802760, 4522050840969728, 6926980500494361616, 9254915170315092113 };

// For bishops
u64* CreatePossibleBishopBlockers(int squareIndex) {
    std::vector<int> shiftQuantity;
    u64 bitmap = 0;
    int maxSize = 0;

    for (int i = 0; i < 64; i++)
    {
        if ((bishopSight[squareIndex] >> i) & 1)
        {
            shiftQuantity.push_back(i);
        }
    }
    maxSize = 1 << shiftQuantity.size();
    u64* bishopAttack = new u64[maxSize];
    for (int i = 0; i < maxSize; i++)
    {
        for (int j = 0; j < shiftQuantity.size(); j++)
        {
            u64 bit = (i >> j) & 1;
            bitmap |= bit << shiftQuantity[j];
        }
        bishopAttack[i] = bitmap;
        bitmap = 0ULL;
    }
    return bishopAttack;
}

void MakeBishopMagics(int squareIndex) {
    //Get Array Of Possible Blockers for current square
    u64* bishopAttack = CreatePossibleBishopBlockers(squareIndex);
    int lowestIndex = 32000;
    int lookedRands = 0;
    u64 bestMagic = 0;
    int bestShift = 0;
    int maxIndex = 0;
    int shiftAmount = 64 - BBits[squareIndex];
    u64 currentIndex = 0;
    int maxSize = 1 << BBits[squareIndex];
    std::unordered_map<int, int> lookUpTable;

    std::ofstream bishopMagics("magics.txt", std::ios::app);
    std::ofstream bishopShifts("shifts.txt", std::ios::app);
    std::ofstream bishopMaxIndex("maxIndex.txt", std::ios::app);
    bishopMagics.clear();
    bishopShifts.clear();
    bishopMaxIndex.clear();
    //Find Magic Number for current square
    while (true)
    {
        lookedRands++;
        if (lowestIndex == maxSize - 1)
        {
            std::cout << "Best Magic Found For Square " << squareIndex << ": " << bestMagic << " Shift Amount: " << bestShift << " Max Index " << lowestIndex << " Random Numbers Checked: " << lookedRands << "\n\n";
            bishopMagics << bestMagic << ", ";
            bishopShifts << bestShift << ", ";
            delete[] bishopAttack;
            return;
        }
        maxIndex = 0;
        u64 randomNumber = randomGenerator() & randomGenerator() & randomGenerator();
        lookUpTable.clear();
        for (int blockerIndex = 0; blockerIndex < maxSize; blockerIndex++)
        {
            currentIndex = ((bishopAttack[blockerIndex] * randomNumber) >> shiftAmount);
            if (lookUpTable[currentIndex] > 0)
            {
                lookUpTable.clear();
                break;
            }
            if (currentIndex > maxIndex) {
                maxIndex = currentIndex;
            }
            lookUpTable[currentIndex]++;
        }
        if ((lookUpTable.size() == maxSize) && (maxIndex < lowestIndex))
        {
            lowestIndex = maxIndex;
            bestMagic = randomNumber;
            bestShift = shiftAmount;
        }
    }
}

u64 FindSlidingBishopMovesMagic(u64 blockers, u64 startIndex, u64 rowConstraint, u64 columnConstraint, int shiftAmount) {
    int negative = 0b10000000000000000000000000000000;
    int offSet = 0;
    int moveCount = 0;
    u64 tempStart = 0;
    u64 targetSquare = 0;
    u64 targetMap = 0;
    u64 startSquare = Chessboard::BitIndexToBitmap(startIndex);
    tempStart = startSquare;
    if ((shiftAmount & negative) == negative) targetSquare = startSquare >> (~shiftAmount + 1);
    else targetSquare = startSquare << shiftAmount;
    if (blockers == 0) return bishopSightPremask[startIndex];
    while (true)
    {
        if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0) && ((targetSquare & ~(blockers)) == targetSquare))
        {
            targetMap |= targetSquare;
            moveCount++;
        }
        else if (((tempStart & rowConstraint) == 0) && ((tempStart & columnConstraint) == 0)) {
            targetMap |= targetSquare;
            break;
        }
        else
        {
            break;
        }
        tempStart = targetSquare;
        if ((shiftAmount & negative) == negative) targetSquare = targetSquare >> (~shiftAmount + 1);
        else targetSquare = targetSquare << shiftAmount;
    }
    return targetMap;
}

u64 CreateTargetSquareMapBishop(u64 startSquare, u64 blockers) {
    u64 targetMap = 0;
    //North West
    targetMap |= FindSlidingBishopMovesMagic(blockers, startSquare, Row::Row8, Column::ColumnA, 7);
    //North East
    targetMap |= FindSlidingBishopMovesMagic(blockers, startSquare, Row::Row8, Column::ColumnH, 9);
    //South West
    targetMap |= FindSlidingBishopMovesMagic(blockers, startSquare, Row::Row1, Column::ColumnA, -9);
    //South East
    targetMap |= FindSlidingBishopMovesMagic(blockers, startSquare, Row::Row1, Column::ColumnH, -7);
    return targetMap;
}

void InitialiseMagicArrays() {
    u64* rookBlockers;
    u64* bishopBlockers;
    for (int square = 0; square < 64; square++)
    {
        int maxSizeRook = 1 << RBits[square];
        int maxSizeBishop = 1 << BBits[square];
        rookBlockers = CreatePossibleRookBlockers(square);
        bishopBlockers = CreatePossibleBishopBlockers(square);
        rookBlockersMagics[square] = new u64[maxSizeRook];
        bishopBlockersMagics[square] = new u64[maxSizeBishop];
        for (int blockerIndex = 0; blockerIndex < maxSizeRook; blockerIndex++)
        {
            u64 targetMap = CreateTargetSquareMapRook(square, rookBlockers[blockerIndex]);
            u64 magicIndex = ((rookMagics[square] * rookBlockers[blockerIndex]) >> rookShifts[square]);
            rookBlockersMagics[square][magicIndex] = targetMap;
        }
        for (int blockerIndex = 0; blockerIndex < maxSizeBishop; blockerIndex++)
        {
            u64 targetMap = CreateTargetSquareMapBishop(square, bishopBlockers[blockerIndex]);
            u64 magicIndex = ((bishopMagics[square] * bishopBlockers[blockerIndex]) >> bishopShifts[square]);
            bishopBlockersMagics[square][magicIndex] = targetMap;
        }
        delete[] rookBlockers;
        delete[] bishopBlockers;
    }
}
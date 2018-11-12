/*
  Ethereal is a UCI chess playing engine authored by Andrew Grant.
  <https://github.com/AndyGrant/Ethereal>     <andrew@grantnet.us>

  Ethereal is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Ethereal is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "assert.h"
#include "bitboards.h"
#include "evaluate.h"
#include "psqt.h"
#include "types.h"

int PSQT[32][SQUARE_NB];

#define S(mg, eg) MakeScore((mg), (eg))

const int PawnPSQT32[32] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( -17,   5), S(  13,   0), S(   0,   0), S(   2,  -3),
    S( -23,   6), S(  -2,   1), S(  -3,  -4), S(   5, -13),
    S( -14,  14), S(  -2,   6), S(  11,  -9), S(  10, -22),
    S(  -4,  17), S(   5,   6), S(   2,  -1), S(   7, -21),
    S(   0,  26), S(   9,  24), S(  14,   2), S(  24, -20),
    S( -44,   6), S( -34,   8), S(  -1, -16), S(   1, -30),
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
    S( -39, -48), S(   0, -37), S( -10, -15), S(   4,  -7),
    S(   5, -49), S(   7, -11), S(   7, -22), S(  12,  -2),
    S(   4, -23), S(  23, -19), S(  14,   2), S(  28,  12),
    S(   3,   5), S(  27,   7), S(  33,  31), S(  51,  33),
    S(  28,   5), S(  44,  12), S(  44,  40), S(  54,  44),
    S( -28,   8), S(  25,   4), S(  41,  37), S(  49,  33),
    S( -34, -22), S( -36,   4), S(  40, -33), S(  13,  -2),
    S(-169, -35), S(-103, -31), S(-157,  -7), S( -40, -27),
};

const int BishopPSQT32[32] = {
    S(  25, -22), S(  19, -27), S(  -3,  -9), S(  18, -14),
    S(  33, -29), S(  27, -25), S(  22, -13), S(   9,  -2),
    S(  23, -11), S(  29, -12), S(  20,   0), S(  21,   7),
    S(   8,  -5), S(  18,   0), S(  18,  13), S(  34,  18),
    S( -14,  11), S(  37,   3), S(   4,  16), S(  30,  19),
    S(  -1,   6), S(   0,   7), S(  26,   7), S(  21,   6),
    S( -69,   1), S(  -3,  -4), S(  -9, -11), S( -40,   0),
    S( -49,   0), S( -62,  -2), S(-126,   3), S(-111,  10),
};

const int RookPSQT32[32] = {
    S(  -1, -30), S(  -8, -16), S(   4, -16), S(  10, -25),
    S( -39, -25), S(  -6, -28), S(   0, -22), S(   6, -30),
    S( -17, -19), S(   5, -14), S(  -3, -20), S(   1, -21),
    S( -17,  -1), S( -10,   4), S(  -2,   2), S(   0,   3),
    S( -11,  12), S(  -9,  10), S(  16,   5), S(  21,   7),
    S( -16,  14), S(  15,   9), S(  10,  13), S(  18,  12),
    S(  -2,  15), S(  -7,  17), S(  34,   1), S(  20,   8),
    S(   0,  23), S(  11,  13), S( -22,  23), S(   3,  27),
};

const int QueenPSQT32[32] = {
    S(   0, -47), S( -13, -30), S(  -3, -21), S(  11, -40),
    S(   7, -47), S(  14, -37), S(  17, -54), S(   9, -14),
    S(   5, -22), S(  23, -17), S(   6,   6), S(   0,   1),
    S(   4,  -5), S(  10,   4), S(  -5,  15), S(  -3,  46),
    S( -10,  10), S( -13,  33), S(  -7,  22), S( -21,  52),
    S( -12,   3), S(  -5,  18), S(   0,  21), S(  -9,  46),
    S(  -4,  12), S( -74,  55), S(  22,  11), S( -19,  66),
    S( -20, -22), S(   2, -12), S(   8,  -4), S( -18,   9),
};

const int KingPSQT32[32] = {
    S(  81,-105), S(  90, -83), S(  35, -34), S(  23, -39),
    S(  66, -53), S(  56, -41), S(   5,   0), S( -15,   7),
    S(   0, -42), S(  44, -28), S(  16,   2), S( -11,  21),
    S( -53, -34), S(  32, -20), S(   1,  18), S( -44,  38),
    S( -19, -19), S(  53,   0), S(   8,  30), S( -29,  38),
    S(  37, -18), S(  82,  -1), S(  73,  18), S(   8,  15),
    S(  14, -17), S(  49,  -4), S(  32,   0), S(   6,   0),
    S(  26, -81), S(  83, -67), S( -22, -35), S( -16, -36),
};

#undef S

int relativeSquare32(int s, int c) {
    assert(0 <= c && c < COLOUR_NB);
    assert(0 <= s && s < SQUARE_NB);
    static const int edgeDistance[FILE_NB] = {0, 1, 2, 3, 3, 2, 1, 0};
    return 4 * relativeRankOf(c, s) + edgeDistance[fileOf(s)];
}

void initializePSQT() {

    for (int s = 0; s < SQUARE_NB; s++) {
        const int w32 = relativeSquare32(s, WHITE);
        const int b32 = relativeSquare32(s, BLACK);

        PSQT[WHITE_PAWN  ][s] = +MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) +   PawnPSQT32[w32];
        PSQT[WHITE_KNIGHT][s] = +MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) + KnightPSQT32[w32];
        PSQT[WHITE_BISHOP][s] = +MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) + BishopPSQT32[w32];
        PSQT[WHITE_ROOK  ][s] = +MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) +   RookPSQT32[w32];
        PSQT[WHITE_QUEEN ][s] = +MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) +  QueenPSQT32[w32];
        PSQT[WHITE_KING  ][s] = +MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) +   KingPSQT32[w32];

        PSQT[BLACK_PAWN  ][s] = -MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) -   PawnPSQT32[b32];
        PSQT[BLACK_KNIGHT][s] = -MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) - KnightPSQT32[b32];
        PSQT[BLACK_BISHOP][s] = -MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) - BishopPSQT32[b32];
        PSQT[BLACK_ROOK  ][s] = -MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) -   RookPSQT32[b32];
        PSQT[BLACK_QUEEN ][s] = -MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) -  QueenPSQT32[b32];
        PSQT[BLACK_KING  ][s] = -MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) -   KingPSQT32[b32];
    }
}

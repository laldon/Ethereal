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
    S( -15,  11), S(   9,   2), S( -14,   5), S( -10,  -3),
    S( -17,   4), S( -12,   5), S(  -7, -10), S(  -5, -18),
    S( -17,  19), S( -10,  14), S(  18, -17), S(  15, -35),
    S(  -3,  24), S(   4,  13), S(   6,  -2), S(  21, -29),
    S(  -8,  41), S(  -9,  37), S(  -1,  17), S(  29, -11),
    S( -13, -15), S( -78,  17), S(  25, -25), S(  49, -27),
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
    S( -72, -10), S( -16, -48), S( -25, -25), S( -10, -28),
    S( -13, -11), S(   1, -24), S( -12, -33), S(   0, -25),
    S(  -6, -26), S(  14, -30), S(   5, -29), S(  13,  -7),
    S(  13,   7), S(   8,  11), S(  20,  14), S(  18,  24),
    S(  24,  22), S(  21,  10), S(  33,  26), S(  20,  43),
    S( -11,  20), S(   0,  11), S(  36,  24), S(  12,  38),
    S(   1,   9), S( -24,  12), S(  11, -10), S(  70,   6),
    S(-144,  -2), S( -62,  24), S(-107,  43), S( -19,  23),
};

const int BishopPSQT32[32] = {
    S(  13, -15), S(  15, -22), S( -20,  -6), S(   5, -13),
    S(  27, -27), S(  19, -28), S(  18, -14), S(   3,  -6),
    S(  11,  -6), S(  24, -12), S(  11,  -3), S(  14,   0),
    S(   7, -10), S(  15,   0), S(  12,   8), S(  11,  14),
    S( -19,  10), S(  14,   5), S(  -1,  14), S(   2,  23),
    S(   1,   2), S( -11,  23), S(  10,  15), S(  15,  15),
    S( -43,  17), S( -50,  13), S(  -2,  13), S( -30,  11),
    S( -33,  14), S( -36,  20), S( -94,  24), S(-118,  31),
};

const int RookPSQT32[32] = {
    S(  -8, -36), S( -21, -20), S(  -6, -27), S(   0, -36),
    S( -71, -17), S( -25, -34), S( -20, -35), S( -11, -39),
    S( -27, -21), S(  -9, -16), S( -30, -18), S( -13, -25),
    S( -14,  -4), S( -11,  13), S( -21,   7), S(  -9,   2),
    S(  -5,  23), S(  17,  23), S(  12,  14), S(  26,   8),
    S( -17,  37), S(  12,  29), S( -14,  37), S(  16,  19),
    S(  -5,  24), S( -24,  36), S( -14,  27), S(  12,  25),
    S(  40,  26), S(  36,  32), S(  23,  39), S(  -4,  43),
};

const int QueenPSQT32[32] = {
    S(  13, -82), S( -26, -27), S(  -9, -60), S(   2, -48),
    S(   6, -28), S(  16, -65), S(  17, -86), S(   1, -19),
    S(  -2, -16), S(  14, -21), S(  -3,   4), S(  -7,  -3),
    S(   0,  -8), S(  -1,  27), S( -17,  35), S( -32,  80),
    S(  -4,  -4), S( -20,  56), S( -34,  43), S( -46,  92),
    S( -35,  31), S( -38,  45), S( -45,  50), S( -21,  32),
    S( -15,  41), S( -69,  82), S( -33,  41), S( -45,  66),
    S(  -1,  41), S(  -3,  44), S( -12,  35), S(   8,  41),
};

const int KingPSQT32[32] = {
    S(  50,-110), S(  50, -75), S(   4, -44), S(  -4, -62),
    S(  32, -49), S(   1, -43), S( -27, -17), S( -32, -18),
    S(  11, -49), S(  13, -41), S(   8, -15), S(  -9,  -2),
    S(  -6, -38), S(  60, -35), S(  23,   5), S( -33,  18),
    S(  -3,  -5), S(  88, -16), S(  35,  24), S( -13,  31),
    S(  61,  -2), S( 120,  -1), S(  99,  30), S(  47,  25),
    S(  17,  -3), S(  51,  12), S(  28,  29), S(   2,  15),
    S(  15, -94), S(  79, -50), S( -13,  -1), S( -17,  18),
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

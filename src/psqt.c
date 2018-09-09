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
    S(  -7,  32), S(  49,  22), S(   9,  23), S(   2,   0), 
    S(   0,  18), S(  36,  17), S(  14,   7), S(  24,   0), 
    S(  -4,  40), S(  18,  35), S(  42,   2), S(  40,  -7), 
    S(  11,  43), S(  34,  31), S(  44,  18), S(  51,  -6), 
    S(   8,  41), S(  17,  36), S(  21,  10), S(  38, -12), 
    S( -41,  12), S( -33,  11), S(   0, -14), S(   2, -28), 
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0), 
};

const int KnightPSQT32[32] = {
    S( -39, -48), S( -44, -42), S( -21, -20), S( -13, -15), 
    S(  -8, -51), S(   2, -14), S( -17, -27), S( -13, -14), 
    S( -33, -26), S(  18, -21), S(   7, -10), S(  26,  11), 
    S(  14,   6), S(  31,   9), S(  56,  35), S(  65,  38), 
    S(  35,   8), S(  74,  19), S(  65,  50), S(  74,  56), 
    S( -26,   9), S(  29,   7), S(  46,  42), S(  54,  39), 
    S( -31, -20), S( -34,   5), S(  41, -30), S(  15,   0), 
    S(-168, -34), S(-102, -30), S(-156,  -6), S( -39, -26), 
};

const int BishopPSQT32[32] = {
    S(  21, -24), S(  16, -29), S( -43, -20), S(   5, -22), 
    S(  31, -30), S(  24, -35), S(  19, -15), S(  -6,  -6), 
    S(  18, -12), S(  29, -13), S(  10,  -2), S(  22,   3), 
    S(  11,  -5), S(  24,   2), S(  29,  17), S(  40,  21), 
    S( -11,  12), S(  56,   8), S(  12,  21), S(  39,  26), 
    S(   4,   7), S(   2,  10), S(  30,  11), S(  25,  10), 
    S( -66,   2), S(  -2,  -2), S(  -4,  -7), S( -38,   1), 
    S( -48,   0), S( -61,  -1), S(-125,   3), S(-110,  10), 
};

const int RookPSQT32[32] = {
    S(  -6, -39), S( -20, -21), S(   5, -28), S(  25, -34), 
    S( -61, -32), S( -10, -33), S( -10, -32), S(  -3, -43), 
    S( -23, -24), S(   3, -16), S( -11, -27), S(  -3, -26), 
    S( -11,  -1), S(  -7,   7), S(  -1,   4), S(   3,   7), 
    S(  -2,  19), S(   0,  18), S(  24,  12), S(  32,  17), 
    S(  -9,  21), S(  21,  16), S(  14,  20), S(  26,  20), 
    S(   4,  23), S(  -2,  24), S(  36,   6), S(  25,  16), 
    S(   3,  26), S(  13,  16), S( -19,  27), S(   5,  31), 
};

const int QueenPSQT32[32] = {
    S(   0, -47), S( -18, -31), S( -17, -25), S( -13, -45), 
    S(   6, -47), S(  11, -38), S(  13, -61), S( -13, -20), 
    S(   3, -22), S(  30, -17), S(   0,   5), S( -13,  -2), 
    S(  11,  -3), S(  21,   7), S(   0,  18), S(  -5,  49), 
    S(   4,  13), S(  -4,  36), S(   0,  25), S( -13,  55), 
    S(  -6,   5), S(  -1,  19), S(   3,  22), S(  -4,  47), 
    S(   1,  13), S( -70,  56), S(  23,  11), S( -17,  66), 
    S( -17, -20), S(   3, -11), S(   8,  -3), S( -16,  10), 
};

const int KingPSQT32[32] = {
    S(  75,-107), S( 116, -82), S(  25, -42), S(  11, -50), 
    S(  61, -54), S(  60, -22), S(  -3,  -1), S( -17,   1), 
    S(  -1, -43), S(  39,  -2), S(  15,   9), S( -18,  22), 
    S( -50, -30), S(  26,   3), S(   1,  28), S( -28,  36), 
    S( -19, -18), S( -14,   4), S(   1,  25), S( -29,  34), 
    S( -19, -19), S( -14,   2), S( -14,  22), S( -24,  27), 
    S( -19, -19), S( -29,   0), S( -34,   0), S( -29,   5), 
    S( -79, -80), S( -60, -60), S( -35, -35), S( -34, -34), 
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

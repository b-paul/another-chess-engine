/*
 * This file is part of Nerd Engine
 *
 * Nerd Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nerd Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with Nerd Engine.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <stdint.h>

typedef uint64_t Bitboard;
typedef uint8_t Castling_Perm;

typedef enum {
    PAWN=1, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    PIECE_TYPE_CNT
} Piece_Type;

typedef enum {
    NO_PIECE,
    wP=1, wN, wB, wR, wQ, wK,
    /* Black pieces have the 8 bit set as a flag */
    bP=9, bN, bB, bR, bQ, bK,
    PIECE_CNT
} Piece;

typedef enum {
    WHITE, BLACK,
    TURN_CNT
} Turn;

typedef enum {
    RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8,
    RANK_CNT
} Rank;

typedef enum {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_CNT
} File;

typedef enum {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    SQ_CNT,
    NO_SQ = 64
} Square;

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

typedef enum {
    NO_PIECE,
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    PIECE_TYPE_CNT
} Piece_Type;

typedef enum {
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
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8,
    SQ_CNT
} Square;

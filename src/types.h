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

typedef Bitboard uint64_t;

typedef enum {
    NO_PIECE,
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    PIECE_TYPE_CNT
} Piece_Type;

typedef enum {
    NO_PIECE,
    wP,   wN, wB, wR, wQ, wK,
    /* Black pieces have the 8 bit set as a flag */
    bP=9, bN, bB, bR, bQ, bK,
    PIECE_CNT
} Piece;

typedef enum {
    WHITE, BLACK,
    TURN_CNT
} Turn;

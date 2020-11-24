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
 * along with Nerd Engine.	If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include "../defs.h"

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

extern Bitboard allowed_squares_by_dir[36];

typedef struct {
	Bitboard pieces[PIECE_TYPE_CNT];
	Bitboard sides[TURN_CNT];

	Piece mailbox[SQ_CNT];

	Turn turn;

	/*
	 * Four bits are used to represent castling perms
	 * Bit 1 is wK, 2 is wQ, 4 is bK and 8 is bQ
	 *
	 */
	Castling_Perm castle_perms;

	Square en_pas_square;

	/* Max is 50 for 50 move rule so uint8 is fine */
	uint8_t half_move_cnt;
} Board;

enum {
	CASTLE_WHITE_KING  = 1 << 0,
	CASTLE_WHITE_QUEEN = 1 << 1,
	CASTLE_BLACK_KING  = 1 << 2,
	CASTLE_BLACK_QUEEN = 1 << 3,
	
	CASTLE_WHITE = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN,
	CASTLE_BLACK = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN,
};

void clear_board(Board *board);
void parse_fen(Board *board, const char *str);
#ifdef DEBUG
void print_board(Board *board);
void print_bitboard(Bitboard b);
#endif

void init_attacks();
Bitboard get_pawn_attacks(Square sq, Turn t);
Bitboard get_knight_attacks(Square sq);
Bitboard get_king_attacks(Square sq);

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
/*
 * This file contains what is used to generate attack bitboards that are used
 * in move generation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "defs.h"
#include "helpers.h"
#include "../defs.h"

Bitboard allowed_squares_by_dir[36];

Bitboard pawn_attacks[SQ_CNT][TURN_CNT];
Bitboard knight_attacks[SQ_CNT];
Bitboard king_attacks[SQ_CNT];

Magic rook_magics[SQ_CNT];
Magic bishop_magics[SQ_CNT];

/* 
 * Initialize bitboards used for shift checking
 * See piece_shift in "board/helpers.h"
 */

void
init_allowed_shift_squares() {
	for (int i = 0; i < 36; i++) {
		switch ((i-18)%8) {
			case 1:
			case -7:
				allowed_squares_by_dir[i] = 0x7F7F7F7F7F7F7F7F;
				break;
			case 2:
			case -6:
				allowed_squares_by_dir[i] = 0x3F3F3F3F3F3F3F3F;
				break;
			case 6:
			case -2:
				allowed_squares_by_dir[i] = 0xFCFCFCFCFCFCFCFC;
				break;
			case 7:
			case -1:
				allowed_squares_by_dir[i] = 0xFEFEFEFEFEFEFEFE;
				break;
			default:
				allowed_squares_by_dir[i] = 0xFFFFFFFFFFFFFFFF;
				break;
		}
	}
}

/* Non-sliding piece attacks */

void
gen_pawn_attacks() {
	for (Square sq = A1; sq <= H8; sq++) {
		pawn_attacks[sq][WHITE]  = piece_shift(1ULL << sq, NORTH + EAST);
		pawn_attacks[sq][WHITE] |= piece_shift(1ULL << sq, NORTH + WEST);
		pawn_attacks[sq][BLACK]  = piece_shift(1ULL << sq, SOUTH + EAST);
		pawn_attacks[sq][BLACK] |= piece_shift(1ULL << sq, SOUTH + WEST);
	}
}

void
gen_knight_attacks() {
	for (Square sq = A1; sq <= H8; sq++) {
		knight_attacks[sq]  = piece_shift(1ULL << sq, NORTH * 2 + EAST);
		knight_attacks[sq] |= piece_shift(1ULL << sq, NORTH * 2 + WEST);

		knight_attacks[sq] |= piece_shift(1ULL << sq, SOUTH * 2 + EAST);
		knight_attacks[sq] |= piece_shift(1ULL << sq, SOUTH * 2 + WEST);

		knight_attacks[sq] |= piece_shift(1ULL << sq, EAST * 2 + NORTH);
		knight_attacks[sq] |= piece_shift(1ULL << sq, EAST * 2 + SOUTH);

		knight_attacks[sq] |= piece_shift(1ULL << sq, WEST * 2 + NORTH);
		knight_attacks[sq] |= piece_shift(1ULL << sq, WEST * 2 + SOUTH);
	}
}

void
gen_king_attacks() {
	for (Square sq = A1; sq <= H8; sq++) {
		king_attacks[sq]  = piece_shift(1ULL << sq, NORTH);
		king_attacks[sq] |= piece_shift(1ULL << sq, SOUTH);
		king_attacks[sq] |= piece_shift(1ULL << sq, EAST);
		king_attacks[sq] |= piece_shift(1ULL << sq, WEST);

		king_attacks[sq] |= piece_shift(1ULL << sq, NORTH + EAST);
		king_attacks[sq] |= piece_shift(1ULL << sq, NORTH + WEST);

		king_attacks[sq] |= piece_shift(1ULL << sq, SOUTH + EAST);
		king_attacks[sq] |= piece_shift(1ULL << sq, SOUTH + WEST);
	}
}

/* Sliding piece attacks */

/*
 * Keep going in each direction from a square and stop when there is a piece
 */
Bitboard
get_sliding_attack(Square sq, Bitboard occ, Direction dirs[4]) {
	Bitboard r = 0ULL;

	int dir_i;

	for (dir_i = 0; dir_i < 4; dir_i++) {
		Bitboard sq_bb = 1ULL << sq;
		do {
			sq_bb = piece_shift(sq_bb, dirs[dir_i]);
			r |= sq_bb;
		} while (!(sq_bb & occ) && sq_bb);
	}

	return r;
}

void
gen_sliding_attacks() {
	Direction bishop_dirs[4] = {
		NORTH + EAST, NORTH + WEST, SOUTH + EAST, SOUTH + WEST
	};
	Direction rook_dirs[4] = {
		NORTH, EAST, SOUTH, WEST
	};

	Square sq;
	Bitboard occ;

	init_magic_numbers();

	for (sq = A1; sq <= H8; sq++) {

		/* Bitboard that is used to get rid of the edges in a mask */
		Bitboard notedges =
			~(((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(sq))) |
			((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(sq))));

		rook_magics[sq].mask   =
			get_sliding_attack(sq, 0ULL, rook_dirs) & notedges;
		bishop_magics[sq].mask =
			get_sliding_attack(sq, 0ULL, bishop_dirs) & notedges;

		rook_magics[sq].shift   = 64 - popcnt(rook_magics[sq].mask);
		bishop_magics[sq].shift = 64 - popcnt(bishop_magics[sq].mask);

		rook_magics[sq].attacks =
			malloc((1ULL << popcnt(rook_magics[sq].mask))   * 8);
		bishop_magics[sq].attacks =
			malloc((1ULL << popcnt(bishop_magics[sq].mask)) * 8);

		/*
		 * This loops over all relevant occupancies and writes the attack for
		 * that case into the attacks table
		 */

		occ = rook_magics[sq].mask;
		do {
			rook_magics[sq].attacks[rook_index(sq, occ)] =
				get_sliding_attack(sq, occ, rook_dirs);
		} while ((occ = (occ - 1) & rook_magics[sq].mask));

		occ = bishop_magics[sq].mask;
		do {
			bishop_magics[sq].attacks[bishop_index(sq, occ)] =
				get_sliding_attack(sq, occ, bishop_dirs);
		} while ((occ = (occ - 1) & bishop_magics[sq].mask));
	}
}

void
init_attacks() {
	init_allowed_shift_squares();
	gen_pawn_attacks();
	gen_knight_attacks();
	gen_king_attacks();
	gen_sliding_attacks();
}

Bitboard
get_pawn_attacks(Square sq, Turn t) {
	assert(valid_square(sq));
	assert(valid_turn(t));
	return pawn_attacks[sq][t];
}

Bitboard
get_knight_attacks(Square sq) {
	assert(valid_square(sq));
	return knight_attacks[sq];
}

Bitboard
get_king_attacks(Square sq) {
	assert(valid_square(sq));
	return king_attacks[sq];
}

Bitboard
get_rook_attacks(Square sq, Bitboard occ) {
	assert(valid_square(sq));
	return rook_magics[sq].attacks[rook_index(sq, occ)];
}

Bitboard
get_bishop_attacks(Square sq, Bitboard occ) {
	assert(valid_square(sq));
	return bishop_magics[sq].attacks[bishop_index(sq, occ)];
}

Bitboard
get_queen_attacks(Square sq, Bitboard occ) {
	assert(valid_square(sq));
	return rook_magics[sq].attacks[rook_index(sq, occ)] |
		bishop_magics[sq].attacks[bishop_index(sq, occ)];
}


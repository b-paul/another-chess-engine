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
#include <assert.h>

#include "defs.h"
#include "helpers.h"
#include "../defs.h"

Bitboard allowed_squares_by_dir[36];

Bitboard pawn_attacks[SQ_CNT][TURN_CNT];
Bitboard knight_attacks[SQ_CNT];
Bitboard king_attacks[SQ_CNT];

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

/* 
 * Generating pawn, knight and king attacks is as simple as shifting a square.
 */

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

void
init_attacks() {
	init_allowed_shift_squares();
	gen_pawn_attacks();
	gen_knight_attacks();
	gen_king_attacks();
}

Bitboard get_pawn_attacks(Square sq, Turn t) {
	assert(valid_square(sq));
	assert(valid_turn(t));
	return pawn_attacks[sq][t];
}

Bitboard get_knight_attacks(Square sq) {
	assert(valid_square(sq));
	return knight_attacks[sq];
}

Bitboard get_king_attacks(Square sq) {
	assert(valid_square(sq));
	return king_attacks[sq];
}

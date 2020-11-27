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

#include <stdbool.h>
#include <assert.h>

/*
 * Helper functions
 */

static inline bool
valid_piece(Piece p) {
	return (p >= wP && p <= wK) ||
		   (p >= bP && p <= bK);
}

static inline bool
valid_file(File f) {
	return f >= FILE_A && f <= FILE_H;
}

static inline bool
valid_rank(Rank r) {
	return r >= RANK_1 && r <= RANK_8;
}

static inline bool
valid_square(Square sq) {
	return sq >= A1 && sq <= H8;
}

static inline bool
valid_turn(Turn t) {
	return t == WHITE || t == BLACK;
}



static inline File
file(Square sq) {
	assert(valid_square(sq));
	return sq & 7;
}

static inline Rank
rank(Square sq) {
	assert(valid_square(sq));
	return (sq >> 3) & 7;
}

static inline Square
square(File f, Rank r) {
	assert(valid_file(f));
	assert(valid_rank(r));
	return r << 3 | f;
}



static inline Piece_Type
piece_type(Piece p) {
	assert(valid_piece(p));
	return p & 7;
}

static inline Turn
piece_side(Piece p) {
	return (p & 8) >> 3;
}

static inline void
place_piece(Board *board, Piece p, Square s) {
	board->mailbox[s] = p;

	board->pieces[piece_type(p)] |= 1ULL << s;
	board->sides [piece_side(p)] |= 1ULL << s;
}



/* 
 * Since leftshifting a negative number is illegal, this function right shifts
 * when needed.
 */
static inline
Bitboard shift(Bitboard b, Shift s) {
	return s > 0 ? b << s : b >> -s;
}

/*
 * Some sqaures cannot move in certain directions because they are on the edge
 * of the board. E.g. A8 moving NORTH EAST would end an B1, but it should just
 * give nothing.
 */

static inline uint8_t
dir_index(Direction dir) {
	return dir+18;
}

static inline Bitboard
piece_shift(Bitboard b, Direction dir) {
	return shift(b & allowed_squares_by_dir[dir_index(dir)], (Shift) dir);
}

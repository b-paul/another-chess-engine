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

#include <assert.h>
#include <stdbool.h>

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



static inline uint8_t
popcnt(Bitboard b) {
	return __builtin_popcountll(b);
}


/* Bitboards for a rank and file */
static inline Bitboard
rank_bb(Rank r) {
    return 0x00000000000000FFULL << (r*8);
}

static inline Bitboard
file_bb(File f) {
    return 0x0101010101010101ULL << f;
}

/* 
 * Since leftshifting a negative number is illegal, this function right shifts
 * when needed.
 */
static inline Bitboard
shift(Bitboard b, Shift s) {
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

/* Magic stuff that takes up too much space*/

extern Magic rook_magics[SQ_CNT];
extern Magic bishop_magics[SQ_CNT];

static inline uint16_t
rook_index(Square sq, Bitboard occ) {
	return (rook_magics[sq].mask & occ) *
		rook_magics[sq].magic >>
		rook_magics[sq].shift;
}

static inline uint16_t
bishop_index(Square sq, Bitboard occ) {
	return (bishop_magics[sq].mask & occ) *
		bishop_magics[sq].magic >>
		bishop_magics[sq].shift;
}

static inline void
init_magic_numbers() {
	uint64_t rook_magic_numbers[SQ_CNT] = {
		0x808000645080c000, 0x208020001480c000, 0x4180100160008048,
		0x8180100018001680, 0x4200082010040201, 0x8300220400010008,
		0x3100120000890004, 0x4080004500012180,	0x1548000a1804008,
		0x4881004005208900, 0x480802000801008,  0x2e8808010008800,
		0x8cd804800240080,  0x8a058002008c0080, 0x514000c480a1001,
		0x101000282004d00,  0x2048848000204000, 0x3020088020804000,
		0x4806020020841240, 0x6080420008102202, 0x10050011000800,
		0xac00808004000200, 0x10100020004,      0x1500020004004581,
		0x4c00180052080,    0x220028480254000,  0x2101200580100080,
		0x407201200084200,  0x18004900100500,   0x100200020008e410,
		0x81020400100811,   0x12200024494,      0x8006c002808006a5,
		0x4201000404000,    0x5402202001180,    0x81001002100,
		0x100801000500,     0x4000020080800400, 0x4005050214001008,
		0x810100118b000042, 0xd01020040820020,  0x140a010014000,
		0x420001500210040,  0x54210010030009,   0x4000408008080,
		0x2000400090100,    0x840200010100,     0x233442820004,
		0x800a42002b008200, 0x240200040009080,  0x242001020408200,
		0x4000801000480480, 0x2288008044000880, 0xa800400020180,
		0x30011002880c00,   0x41110880440200,   0x2001100442082,
		0x1a0104002208101,  0x80882014010200a,  0x100100600409,
		0x2011048204402,    0x12000168041002,   0x80100008a000421,
		0x240022044031182
	};
    uint64_t bishop_magic_numbers[SQ_CNT] = {
		0x88b030028800d040, 0x18242044c008010,  0x10008200440000,
		0x4311040888800a00, 0x1910400000410a,   0x2444240440000000,
		0xcd2080108090008,  0x2048242410041004, 0x8884441064080180,
		0x42131420a0240,    0x28882800408400,   0x204384040b820200,
		0x402040420800020,  0x20910282304,      0x96004b10082200,
		0x4000a44218410802, 0x808034002081241,  0x101805210e1408,
		0x9020400208010220, 0x820050c010044,    0x24005480a00000,
		0x200200900890,     0x808040049c100808, 0x9020202200820802,
		0x410282124200400,  0x90106008010110,   0x8001100501004201,
		0x104080004030c10,  0x80840040802008,   0x2008008102406000,
		0x2000888004040460, 0xd0421242410410,   0x8410100401280800,
		0x801012000108428,  0x402080300b04,     0xc20020080480080,
		0x40100e0201502008, 0x4014208200448800, 0x4050020607084501,
		0x1002820180020288, 0x800610040540a0c0, 0x301009014081004,
		0x2200610040502800, 0x300442011002800,  0x1022009002208,
		0x110011000202100,  0x1464082204080240, 0x21310205800200,
		0x814020210040109,  0xc102008208c200a0, 0xc100702128080000,
		0x1044205040000,    0x1041002020000,    0x4200040408021000,
		0x4004040c494000,   0x2010108900408080, 0x820801040284,
		0x800004118111000,  0x203040201108800,  0x2504040804208803,
		0x228000908030400,  0x10402082020200,   0xa0402208010100,
		0x30c0214202044104
    };

	for (Square sq = A1; sq <= H8; sq++) {
		rook_magics[sq].magic   = rook_magic_numbers[sq];
		bishop_magics[sq].magic = bishop_magic_numbers[sq];
	}
}

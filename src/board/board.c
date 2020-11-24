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
 * This file contains functions for initializing positions along with debug
 * functions specifically about the board itself
 */

#include <stdio.h>
#include <assert.h>

#include "defs.h"
#include "helpers.h"
#include "../defs.h"

#define FEN_PARSE_ERROR (0)

#define FEN_ERROR() assert(FEN_PARSE_ERROR); \
	printf("There was an error parsing that fen\n");

void
clear_board(Board *board) {
	for (Piece_Type p = PAWN; p <= KING; p++)
		board->pieces[p] = 0ULL;

	for (Turn t = WHITE; t <= BLACK; t++)
		board->sides[t] = 0ULL;

	for (Square s = A1; s <= H8; s++)
		board->mailbox[s] = NO_PIECE;

	board->turn = WHITE;

	board->castle_perms = 0;
	board->en_pas_square = NO_SQ;

	board->half_move_cnt = 0;
}

void
parse_fen(Board *board, const char *str) {

	/*
	 * 1. Parse pieces
	 *
	 * The first part of a fen string is the pieces position.
	 * The pieces are represented by a letter, P being a pawn, N being a
	 * knight, etc.
	 * Uppercase letters represent white pieces and lowercase letters are
	 * for black.
	 * The fen gives information about what pieces are on each rank, starting
	 * at the 8th rank going from files A to H.
	 * When there is a number it means that there are that many empty squares
	 * until either the next piece or a new rank.
	 * The ranks are separated by the '/' character
	 *
	 */

	Square sq;

	for (sq = A8; *str && (int)sq >= A1; sq++) {

		switch(*str) {
			case 'P':
				place_piece(board, wP, sq);
				break;
			case 'N':
				place_piece(board, wN, sq);
				break;
			case 'B':
				place_piece(board, wB, sq);
				break;
			case 'R':
				place_piece(board, wR, sq);
				break;
			case 'Q':
				place_piece(board, wQ, sq);
				break;
			case 'K':
				place_piece(board, wK, sq);
				break;

			case 'p':
				place_piece(board, bP, sq);
				break;
			case 'n':
				place_piece(board, bN, sq);
				break;
			case 'b':
				place_piece(board, bB, sq);
				break;
			case 'r':
				place_piece(board, bR, sq);
				break;
			case 'q':
				place_piece(board, bQ, sq);
				break;
			case 'k':
				place_piece(board, bK, sq);
				break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				sq += *str - '1';
				break;

			case '/':
				break;

			default:
				FEN_ERROR();
				break;
		}

		/* If the current rank is the H rank, move down and skip the / char */
		if (file(sq) == FILE_H) {
			sq -= 16;
			str++;
		}

		str++;
	}

	/* 
	 * 2. Parse turn
	 *
	 * Simply parse which turn is the current turn for the position based on
	 * either a w for white or b for black.
	 *
	 */

	assert(*str == 'w' || *str == 'b');
	board->turn = (*str == 'w') ? WHITE : BLACK;

	str += 2;

	/*
	 * 3. Parse castling rights
	 *
	 * Parse the castling rights. A K means white can castle kingside, a Q for
	 * queenside, and lowercase for these letters means the same but for black.
	 *
	 */
	while(*str != ' ') {
		switch(*str) {
			case 'K':
				board->castle_perms |= CASTLE_WHITE_KING;
				break;
			case 'Q':
				board->castle_perms |= CASTLE_WHITE_QUEEN;
				break;
			case 'k':
				board->castle_perms |= CASTLE_BLACK_KING;
				break;
			case 'q':
				board->castle_perms |= CASTLE_BLACK_QUEEN;
				break;

			default:
				FEN_ERROR();
				break;
		}
		str++;
	}

	str++;

	/*
	 * 4. Parse en passant square
	 *
	 * The square that a pawn can move to perform an en passant capture is given
	 * next.
	 *
	 */

	if (*str != '-') {
		board->en_pas_square = *str - 'a';
		str++;
		board->en_pas_square = square((File)board->en_pas_square, *str - '1');
	}

	str+=2;

	/*
	 * 5. Parse the half move counter
	 *
	 * This is just a number so all that's needed is conversion from a string 
	 * to an int.
	 *
	 */
	board->half_move_cnt = 10 * (*str - '0');
	str++;
	if (*str != ' ')
		board->half_move_cnt += *str - '0';


	/* At the end there will be a full move counter but that isn't needed */
}

#ifdef DEBUG
void
print_board(Board *board) {
	
	Square sq;

	printf("  A B C D E F G H\n\n");
	for (sq = A8; (int)sq >= A1; sq++) {
		if (file(sq) == FILE_A)
			printf("%c ", "12345678"[rank(sq)]);
		printf("%c", ".PNBRQK  pnbrqk"[board->mailbox[sq]]);

		if (file(sq) == FILE_H) {
			sq -= 16;
			if ((int)sq + 1 >= A1)
				printf(" %c\n  -+-+-+-+-+-+-+- \n", "12345678"[rank(sq+16)]);
		} else
			printf("|");
	}
	printf(" 1\n\n	A B C D E F G H\n\n");
	printf("%s to move\n\n", board->turn == WHITE ? "White" : "Black");
}

void
print_bitboard(Bitboard b) {

	for (int i = 63; i >= 0; i--) {
		if (b & (1ULL << i))
			printf("X");
		else
			printf(".");

		if (i % 8 == 0)
			printf("\n");
	}
}
#endif

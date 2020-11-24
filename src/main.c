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
 * This file contains the UCI loop, which is used to communicate
 * with a GUI through the UCI protocol.
 *
 * The UCI protocol can be found at
 * http://wbec-ridderkerk.nl/html/UCIProtocol.html
 */

#include "board/defs.h"
#include "board/helpers.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char version_str[] = "0.1";

/* Check if a string contains a certain command */
bool
is_uci_command(char *str1, char *str2) {
	return strncmp(str1, str2, strlen(str2)) == 0;
}

/* Print engine information when the uci command is parsed */
void
print_uci_info() {
	printf("id name Nerd Engine %s\n", version_str);
	printf("id author Benjamin Paul\n");
	printf("uciok\n");
}

void parse_position(Board *board, char *str) {
	clear_board(board);

	str += 9;

	if (is_uci_command(str, "fen"))
		parse_fen(board, str+4);
	else if (is_uci_command(str, "startpos"))
		parse_fen(board, STARTING_FEN);
}

int
main() {

	init_attacks();

	/* Remove the need to flush stdio */
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char str[2048];

	Board board;

	clear_board(&board);
	parse_fen(&board, STARTING_FEN);

	/* UCI Loop */
	while(fgets(str, 2048, stdin)) {

		if (is_uci_command(str, "isready"))
			printf("readyok\n");

		else if (is_uci_command(str, "quit"))
			break;

		else if (is_uci_command(str, "uci"))
			print_uci_info();

		else if (is_uci_command(str, "position"))
			parse_position(&board, str);

#ifdef DEBUG
		else if (is_uci_command(str, "print"))
			print_board(&board);
#endif

	}

	return 0;
}

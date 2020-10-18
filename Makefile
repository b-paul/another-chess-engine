EXE=nerdengine
SOURCE=src/main.o src/board/board.o

STANDARD_FLAGS=-O3 -Wall -Wextra -Wshadow -Werror
CFLAGS=${STANDARD_FLAGS}

${EXE}: ${SOURCE}
	${CC} ${CFLAGS} $? -o ${EXE}

all: ${EXE}

clean:
	rm ${SOURCE} ${EXE}

.PHONY: 
	all clean
